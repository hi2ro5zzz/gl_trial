#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define MAXPOINTS 100 /* 記憶する点の数 */
GLint point[MAXPOINTS][2]; /* 座標を記憶する配列 */
int pointnum = 0; /* 記憶した座標の数 */
int rubberband = 0; /* ラバーバンド(マウスのドラッグ中に線分をマウスに追従して描くようにする効果) */

void display(void)
{
    int i;

    /* ウィンドウの塗りつぶし */
    glClear(GL_COLOR_BUFFER_BIT);

    /* 記憶したデータで線を描く */
    if(pointnum > 1) {
        glColor3d(0.0,0.0,0.0);
        glBegin(GL_LINES);
        for (i = 0; i < pointnum; ++i) {
            glVertex2iv(point[i]);
        }
        glEnd();
    }

    /* まだ実行されていないOpenGLの命令を全部実行する */
    glFlush();
}

void resize(int w, int h)
{
    /* ウィンドウ全体をビューポート(実際に描画される領域)にする */
    glViewport(0,0,w,h);

    /* 変換行列の初期化 */
    glLoadIdentity();

    /* スクリーン上の座標系をマウスの座標系に一致させる */
    glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        /* ボタンを操作した位置を記録する */
        point[pointnum][0] = x;
        point[pointnum][1] = y;
        if(state == GLUT_UP) {
            /* ボタンを押した位置から離した位置まで線を引く */
            glColor3d(0.0,0.0,0.0);
            glBegin(GL_LINES);

            /* 2次元の座標値を設定(引数: GLint型の2次元配列) */
            glVertex2iv(point[pointnum - 1]); /* ボタンを押した位置 */
            glVertex2iv(point[pointnum]); /* ボタンを離した位置 */
            glEnd();
            glFlush();

            /* 始点ではラバーバンドを描いていないので消さない */
        }
        else {
    
        }
        if (pointnum < MAXPOINTS - 1) ++pointnum;
        break;
    case GLUT_MIDDLE_BUTTON:
        break;
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }
}

void motion(int x, int y)
{
    static GLint savepoint[2]; /* 以前のラバーバンドの端点 */

    /* 論理演算機能 ON */

    /* 引数に指定した機能を使用可能にする */
    /* GL_COLOR_LOGIC_OP は図形の描画の際にウィンドウにすでに描かれている内容とこれから描こうとする内容の間で論理演算ができるようにする機能 */
    glEnable(GL_COLOR_LOGIC_OP);

    /* 引数にはウィンドウに描かれている内容とこれから描こうとする内容との間で行う論理演算のタイプを指定 */
    /* GL_INVERT はウィンドウに描かれている内容のこれから描こうとする図形の領域を反転する */
    glLogicOp(GL_INVERT);

    glBegin(GL_LINES);
    if (rubberband) {
        /* 以前のラバーバンドを消す */
        glVertex2iv(point[pointnum - 1]);
        glVertex2iv(savepoint);
    }
    /* 新しいラバーバンドを描く */
    glVertex2iv(point[pointnum - 1]);
    glVertex2i(x,y);
    glEnd();

    glFlush();

    /* 論理演算機能 OFF */

    /* これから描こうとする内容をそのままウィンドウ内に描く */
    glLogicOp(GL_COPY);

    /* 引数に指定した機能を使用不可能にする */
    glDisable(GL_COLOR_LOGIC_OP);

    /* 今描いたラバーバンドの端点を保存 */
    savepoint[0] = x;
    savepoint[1] = y;

    /* 今描いたラバーバンドは次のタイミングで消す */
    rubberband = 1;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'q':
    case 'Q':
    case '\033': /* '\033' は ESC の ASCII コード */
        exit(0);
    default:
        break;
    }
}

void init(void)
{
    /* glClear(GL_COLOR_BUFFER_BIT)でウィンドウを塗りつぶす際の色を指定 */
    glClearColor(1.0,1.0,1.0,1.0);
}

int main(int argc, char *argv[])
{
    /* 新たに開くウィンドウの位置を指定 */
    glutInitWindowPosition(100,100);

    /* 新たに開くウィンドウの幅と高さを指定 */
    glutInitWindowSize(320,240);

    /* GLUT及びOpenGL環境を初期化 */
    glutInit(&argc, argv);

    /* ディスプレイの表示モードの設定 */
    glutInitDisplayMode(GLUT_RGBA);

    /* ウィンドウを開く */
    glutCreateWindow(argv[0]);

    /* 開かれたウィンドウ内に描画する関数へのポインタ */
    glutDisplayFunc(display);

    /* 引数にはウィンドウがリサイズされた時に実行する関数のポインタを与える */
    glutReshapeFunc(resize);

    /* 引数にはマウス・ボタンが押された時に実行する関数のポインタを与える */
    glutMouseFunc(mouse);

    /* 引数にはマウスのいずれかのボタンを押しながらマウスを動かした時に実行する関数のポインタを与える */
    glutMotionFunc(motion);

    /* 引数にはキーがタイプされた時に実行する関数のポインタを与える */
    glutKeyboardFunc(keyboard);

    init();

    /* 無限ループ(イベントの待受状態) */
    glutMainLoop();

    return 0;
}