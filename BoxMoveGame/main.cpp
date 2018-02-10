#include <iostream>
using namespace std;

// #壁 _空間　.ゴール oブロック p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int gStageWidth  = 8;
const int gStageHeight = 5;

enum Object {
	SPACE,
	WALL,
	GOAL,
	BLOCK,
	BLOCK_ON_GOAL,
	MAN,
	MAN_ON_GOAL,

	UNKNOWN
};

// 関数プロトタイプ
void initialize(Object*, int, int, const char*);
void draw(const Object*, int, int);
void update(Object*, char, int, int);
bool checkClear(const Object*, int, int);

// メイン関数
int main()
{
	Object *state = new Object[gStageWidth * gStageHeight];

	initialize(state, gStageWidth, gStageHeight, gStageData);

	// ゲームメインループ
	while (true)
	{
		// 描画処理
		draw(state, gStageWidth, gStageHeight);

		// クリア判定
		if (checkClear(state, gStageWidth, gStageHeight))
			break;

		// 入力取得
		cout << "a: 左 d: 右 w: 上 s: 下 コマンドを入力してください。" << endl;
		char input;
		cin >> input;

		// ステージの状態の更新処理
		update(state, input, gStageWidth, gStageHeight);
	}

	// クリアの際のメッセージ
	cout << "おめでとう。あなたの勝ちです。" << endl;

	// 後始末
	delete[] state;

	state = 0;

	return 0;
}


// ステージの初期化
void initialize(Object *state, int width, int height, const char *stageData)
{
	const char *d = stageData;
	int x=0, y=0;

	while (*d != '\0')
	{
		Object tmp;
		switch (*d)
		{
		case '#': tmp = WALL;			 break;
		case ' ': tmp = SPACE;			 break;
		case 'o': tmp = BLOCK;			 break;
		case 'O': tmp = BLOCK_ON_GOAL;	 break;
		case '.': tmp = GOAL;			 break;
		case 'p': tmp = MAN;			 break;
		case 'P': tmp = MAN_ON_GOAL;	 break;
		case '\n':x=0; ++y; tmp=UNKNOWN; break;
		default:  tmp = UNKNOWN;
		}

		++d;
		if (tmp != UNKNOWN)
		{
			state[y*width + x] = tmp;
			++x;
		}
	}
}

// ステージの描画
void draw(const Object *state, int width, int height)
{
	const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};

	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			Object obj = state[y*width + x];
			cout << font[obj];
		}

		cout << endl;
	}
}

// 盤面の状態を更新
void update(Object *state, char input, int width, int height)
{
	// 移動差分
	int diffMoveX=0, diffMoveY=0;

	switch (input)
	{
	case 'a': diffMoveX = -1;	break;	// 左
	case 'd': diffMoveX =  1;	break;	// 右
	case 'w': diffMoveY = -1;	break;	// 上
	case 's': diffMoveY =  1;	break;	// 下
	}

	// プレイヤの検索
	int i = -1;
	for (i=0; i<width*height; ++i)
	{
		if (state[i]==MAN || state[i]==MAN_ON_GOAL)
			break;
	}

	int x = i%width;
	int y = i/width;

	// 移動処理
	// 1マス先の座標
	int moveTargetX = x+diffMoveX;
	int moveTargetY = y+diffMoveY;

	// ステージの範囲外か判定
	if (moveTargetX<0 ||
		moveTargetY<0 ||
		moveTargetX >= width ||
		moveTargetY >= width)
		return;

	// A.その方向が空白またはゴールの場合は人の移動を許可
	int manPos = y*width + x;
	int TargetPos = moveTargetX*width + moveTargetY;

	if (state[TargetPos]==SPACE || state[TargetPos]==GOAL)
	{
		cout << "SPACE or GOAL" << endl;
		state[TargetPos] = (state[TargetPos]==GOAL) ? MAN_ON_GOAL : MAN;
		state[manPos] = (state[manPos]==MAN_ON_GOAL) ? GOAL : SPACE;
	}
	// B. その方向が箱。その方向の次のますが空白またはゴールであれば移動を許可
	else if (state[TargetPos]==BLOCK || state[TargetPos]==BLOCK_ON_GOAL)
	{
		cout << "BLOCK or BLOCK_ON_GOAL" << endl;
		// 2マス先のポインタ
		int twoMovePosX = TargetPos + diffMoveX;
		int twoMovePosY = TargetPos + diffMoveY;

		// 2マス先が範囲内か判定
		if (twoMovePosX<0 || twoMovePosY<0 || twoMovePosX>=width || twoMovePosY>=width)
		{
			return;
		}

		int twoMovePos = (moveTargetY+diffMoveX)*width * (moveTargetX+diffMoveX);
		if (state[twoMovePos]==SPACE || state[twoMovePos]==GOAL)
		{
			// 順番に3マス分を入れ替える
			state[twoMovePos] = (state[twoMovePos]==GOAL) ? BLOCK_ON_GOAL : BLOCK;
			state[TargetPos] = (state[TargetPos]==BLOCK_ON_GOAL) ? MAN_ON_GOAL : MAN;
			state[manPos] = (state[manPos]==MAN_ON_GOAL) ? GOAL : SPACE;
		}
	}
	else
	{
		cout << "default" << endl;
	}
}

// クリアチェック判定
bool checkClear(const Object *state, int width, int height)
{
	for (int i=0; i<width; ++i)
	{
		if (state[i] != BLOCK)
			return false;
	}

	return true;
}
