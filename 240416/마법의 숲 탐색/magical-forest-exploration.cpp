#include <iostream>
#include <queue>
#include <tuple>
#include <cmath>
using namespace std;

#define MAXRC 75
#define MAXK 1010
#define START_R_MAP 4
#define BLANK 0

#define DOWN 1
#define LEFT 2
#define RIGHT 3

// 1-3는 정려이 시작할 열, 본 맵은 4~R+3까지
int Map[MAXRC][MAXRC];

int tmp[3][3];

int R, C, K;

int sum_row;

int cur_r, cur_c, cur_d;

int k;

int vK[MAXK];

struct GOLEM
{
    int r;
    int c;
    int id;
    int door_dir;
} GOLEM_LIST[MAXK];

// 북, 동, 남, 서
int xx[4] = {-1, 0, 1, 0};
int yy[4] = {0, 1, 0, -1};

int golem_cord[5][2] = {
    {0, 0},
    {1, 0},
    {-1, 0},
    {0, 1},
    {0, -1}};

void printMap()
{
    cout << "---------printMap----------" << endl;
    for (int i = START_R_MAP; i < R + START_R_MAP; ++i)
    {
        for (int j = 1; j <= C; ++j)
        {
            cout << Map[i][j] << " ";
        }
        cout << endl;
    }
    cout << "---------end----------\n"
         << endl;
}

void clearMap()
{
    for (int i = 1; i < R + START_R_MAP; ++i)
    {
        for (int j = 1; j <= C; ++j)
        {
            Map[i][j] = 0;
        }
    }
}

void init()
{
    cin >> R >> C >> K;

    clearMap();

    for (int i = 1; i <= K; ++i)
    {
        vK[i] = 0;
    }
    sum_row = 0;
}

void init_golem(int c, int d)
{
    cur_r = 2;
    cur_c = c;
    cur_d = d;
    Map[2][c] = k;

    for (int dd = 0; dd < 4; ++dd)
    {
        Map[cur_r + xx[dd]][cur_c + yy[dd]] = k;
    }

    Map[cur_r + xx[d]][cur_c + yy[d]] = -k;
}

void cpyToTmp()
{
    for (int i = 0; i < 5; ++i)
    {
        tmp[golem_cord[i][0] + 1][golem_cord[i][1] + 1] = Map[cur_r + golem_cord[i][0]][cur_c + golem_cord[i][1]];
        Map[cur_r + golem_cord[i][0]][cur_c + golem_cord[i][1]] = BLANK;
    }
}
void cpyToMap()
{
    for (int i = 0; i < 5; ++i)
    {

        Map[cur_r + golem_cord[i][0]][cur_c + golem_cord[i][1]] = tmp[golem_cord[i][0] + 1][golem_cord[i][1] + 1];
    }
}

// 중심 기준으로 할 것이기 때문에 가운데 기준으로 3X3이 들어갈 수 있는지 확인
bool isRangeGolem(int r, int c)
{
    if (r < R + 3 && r > 1 && c < C && c > 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool canGo(int next_r, int next_c)
{
    bool flag = false;
    if (isRangeGolem(next_r, next_c))
    {
        flag = true;
        for (int i = 0; i < 5; ++i)
        {
            if (Map[next_r + golem_cord[i][0]][next_c + golem_cord[i][1]] != BLANK)
            {
                flag = false;
                break;
            }
        }
    }

    return flag;
}

bool canDown()
{

    cpyToTmp();

    bool flag = canGo(cur_r + 1, cur_c);

    cpyToMap();

    return flag;
}

bool canLeft()
{

    cpyToTmp();

    bool flag = (canGo(cur_r, cur_c - 1) & canGo(cur_r + 1, cur_c - 1));

    cpyToMap();

    return flag;
}

bool canRight()
{

    cpyToTmp();

    bool flag = (canGo(cur_r, cur_c + 1) & canGo(cur_r + 1, cur_c + 1));

    cpyToMap();

    return flag;
}

int canMove()
{
    if (canDown())
    {
        return DOWN;
    }
    else if (canLeft())
    {
        return LEFT;
    }
    else if (canRight())
    {
        return RIGHT;
    }
    else
    {
        return 0;
    }
}

// rotate == -1 반시계, rotate == 1 시계
void rotateGolem(int rotate)
{
    int t;
    if (rotate == -1)
    {
        // 반시계
        t = tmp[0][1];
        tmp[0][1] = tmp[1][2];
        tmp[1][2] = tmp[2][1];
        tmp[2][1] = tmp[1][0];
        tmp[1][0] = t;
        cur_d = (cur_d + 3) % 4;
    }
    else if (rotate == 1)
    {
        // 시계
        t = tmp[0][1];
        tmp[0][1] = tmp[1][0];
        tmp[1][0] = tmp[2][1];
        tmp[2][1] = tmp[1][2];
        tmp[1][2] = t;
        cur_d = (cur_d + 1) % 4;
    }
}

void moveGolem(int move_dir)
{
    cpyToTmp();

    if (move_dir == DOWN)
    {
        cur_r += 1;
    }
    else if (move_dir == LEFT)
    {
        cur_r += 1;
        cur_c -= 1;
        rotateGolem(-1);
    }
    else if (move_dir == RIGHT)
    {
        cur_r += 1;
        cur_c += 1;
        rotateGolem(1);
    }

    cpyToMap();
}

bool isRangeSoul(int x, int y)
{
    if (x >= START_R_MAP && x < R + START_R_MAP && y > 0 && y <= C)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int moveSoul()
{
    int cx, cy, ck, nx, ny, nk;

    int max_r = 0;

    cx = cur_r;
    cy = cur_c;
    ck = k;
    // x, y, k
    queue<tuple<int, int, int>> q;
    q.push(make_tuple(cx, cy, ck));

    vK[k] = k;

    while (!q.empty())
    {
        tie(cx, cy, ck) = q.front();
        q.pop();

        if (cx + 1 > max_r)
        {
            max_r = cx + 1;
        }

        cx += xx[GOLEM_LIST[ck].door_dir];
        cy += yy[GOLEM_LIST[ck].door_dir];

        for (int dd = 0; dd < 4; ++dd)
        {
            nx = cx + xx[dd];
            ny = cy + yy[dd];

            if (isRangeSoul(nx, ny) && Map[nx][ny] != BLANK && Map[nx][ny] != ck)
            {
                nk = abs(Map[nx][ny]);

                if (vK[nk] != k)
                {
                    vK[nk] = k;

                    q.push(make_tuple(GOLEM_LIST[nk].r, GOLEM_LIST[nk].c, nk));
                }
            }
        }
    }

    return max_r - 3;
}

void simulation()
{
    int c, d;
    int move_dir;
    for (k = 1; k <= K; ++k)
    {
        cin >> c >> d;

        init_golem(c, d);

        // 움직일수 있다면 계속 반복
        while (move_dir = canMove())
        {
            moveGolem(move_dir);
        }

        // printMap();
        // 만약 바깥에 있다면? 맵이 시작하는 R좌표보다 1 더 들어와야 모든 부분이 들어옴
        if (cur_r < START_R_MAP + 1)
        {
            clearMap();
            continue;
        }

        GOLEM_LIST[k].id = k;
        GOLEM_LIST[k].r = cur_r;
        GOLEM_LIST[k].c = cur_c;
        GOLEM_LIST[k].door_dir = cur_d;

        // 정령 이동
        int max_row = moveSoul();
        sum_row += max_row;
    }
}

int main(void)
{

    init();
    simulation();

    cout << sum_row << endl;
}