#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

#define MAPSIZE 5

int Map[MAPSIZE][MAPSIZE];
int tempMap[MAPSIZE][MAPSIZE];
int v[MAPSIZE][MAPSIZE];
int piece[25][2];
int pCnt;

int M, K;

int k, m;

int vCnt;

int maxR, maxC, maxD, maxCnt;

vector<int> wall;
vector<int> score;

int xx[4] = {1, -1, 0, 0};
int yy[4] = {0, 0, -1, 1};

void printMap()
{
    for (int i = 0; i < MAPSIZE; ++i)
    {
        for (int j = 0; j < MAPSIZE; ++j)
        {
            cout << Map[i][j] << " ";
        }
        cout << endl;
    }
}

bool isRange(int x, int y)
{
    if (x >= 0 && x < MAPSIZE && y >= 0 && y < MAPSIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void check(int r, int c)
{
    queue<tuple<int, int>> q;
    q.push(make_tuple(r, c));
    int nr, nc;
    int target = Map[r][c];

    int count = 0;
    while (!q.empty())
    {
        tie(r, c) = q.front();
        q.pop();

        piece[pCnt + count][0] = r;
        piece[pCnt + count][1] = c;

        ++count;
        for (int dd = 0; dd < 4; ++dd)
        {
            nr = r + xx[dd];
            nc = c + yy[dd];

            if (v[nr][nc] != vCnt && isRange(nr, nc) && Map[nr][nc] == target)
            {
                v[nr][nc] = vCnt;
                q.push(make_tuple(nr, nc));
            }
        }
    }

    if (count >= 3)
    {
        pCnt += count;
    }
}

void getNewPiece()
{
    for (int j = 0; j < MAPSIZE; ++j)
    {
        for (int i = MAPSIZE - 1; i >= 0; --i)
        {
            if (Map[i][j] == -1)
            {
                Map[i][j] = wall[m++];
            }
        }
    }
}

void delPiece()
{
    score[k] += pCnt;
    int r, c;
    for (int i = 0; i < pCnt; ++i)
    {
        r = piece[i][0];
        c = piece[i][1];

        Map[r][c] = -1;
    }
}

// 현재 맵 상태에서 유물이 있는지 확인
bool checkMap()
{
    pCnt = 0;
    for (int i = 0; i < MAPSIZE; ++i)
    {
        for (int j = 0; j < MAPSIZE; ++j)
        {
            if (v[i][j] != vCnt)
            {
                v[i][j] = vCnt;
                check(i, j);
            }
        }
    }
    ++vCnt;

    if (pCnt > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// r, c 를 기준으로 시계방향으로 90도 회전
void rotate(int r, int c)
{
    int sX = r - 1;
    int sY = c - 1;
    int eX = r + 1;
    int eY = c + 1;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            tempMap[sX + j][sY + (2 - i)] = Map[sX + i][sY + j];
        }
    }

    for (int i = sX; i <= eX; ++i)
    {
        for (int j = sY; j <= eY; ++j)
        {
            Map[i][j] = tempMap[i][j];
        }
    }
}

void simulation()
{
    bool flag;
    for (k = 0; k < K; ++k)
    {
        flag = false;
        maxCnt = 0;
        maxD = 4;
        maxR = maxC = 10;
        for (int r = 1; r <= 3; ++r)
        {
            for (int c = 1; c <= 3; ++c)
            {
                // 90, 180, 270 회전시키면서 확인
                for (int d = 1; d <= 3; ++d)
                {
                    rotate(r, c);
                    if (checkMap())
                    {
                        flag = true;
                        if (maxCnt < pCnt)
                        {
                            maxCnt = pCnt;
                            maxR = r;
                            maxC = c;
                            maxD = d;
                        }
                        else if (maxCnt == pCnt)
                        {
                            if (maxD > d)
                            {
                                maxCnt = pCnt;
                                maxR = r;
                                maxC = c;
                                maxD = d;
                            }
                            else if (maxD == d)
                            {
                                if (maxC > c)
                                {
                                    maxCnt = pCnt;
                                    maxR = r;
                                    maxC = c;
                                    maxD = d;
                                }
                                else if (maxC == c)
                                {
                                    if (maxR > r)
                                    {
                                        maxCnt = pCnt;
                                        maxR = r;
                                        maxC = c;
                                        maxD = d;
                                    }
                                }
                            }
                        }
                    }
                }
                // 다시 돌리면 360도 회전
                rotate(r, c);
            }
        }
        // 회전을 해도 답이 없다면
        if (flag == false)
        {
            return;
        }

        // 선택된 만큼 회전
        for (int d = 0; d < maxD; ++d)
        {
            rotate(maxR, maxC);
        }

        checkMap();
        delPiece();

        getNewPiece();

        // cout << maxR << "," << maxC << ", " << maxD << "---" << maxCnt << endl;

        while (checkMap())
        {
            delPiece();
            getNewPiece();
        }
    }
}

void init()
{
    cin >> K >> M;

    for (int i = 0; i < MAPSIZE; ++i)
    {
        for (int j = 0; j < MAPSIZE; ++j)
        {
            cin >> Map[i][j];
            v[i][j] = 0;
            piece[i][j];
        }
    }
    k = 0;
    m = 0;
    wall.resize(M, 0);
    score.resize(K, 0);
    vCnt = 1;
    for (int i = 0; i < M; ++i)
    {
        cin >> wall[i];
    }
}

int main(void)
{
    //freopen("input.txt", "r", stdin);

    init();

    simulation();

    for (int i = 0; i < K; ++i)
    {
        if (score[i] <= 0)
        {
            cout << endl;
            break;
        }
        cout << score[i] << " ";
    }
    return 0;
}