#include <stdlib.h>
#include <GL/glut.h>

GLdouble vertex[][3] = {
    { 0.0, 0.0, 0.0 },
    { 1.0, 0.0, 0.0 },
    { 1.0, 1.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 1.0, 1.0 },
    { 0.0, 1.0, 1.0 }
};

int face[][4] = {
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 }
};

GLdouble normal[][3] = {
    { 0.0, 0.0,-1.0 },
    { 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    {-1.0, 0.0, 0.0 },
    { 0.0,-1.0, 0.0 },
    { 0.0, 1.0, 0.0 }
};

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };

void cube(void)
{
    int i;
    int j;

    glBegin(GL_QUADS);
    for (j = 0; j < 6; ++j) {
        glNormal3dv(normal[j]);
        for (i = 0; i < 4; ++i) {
            glVertex3dv(vertex[face[j][i]]);
        }
    }
    glEnd();
}

void idle(void)
{
    /* 再描画イベントを発生させる */
    /* glutDisplayFunc() で指定されている描画関数が実行される */
    glutPostRedisplay();
}

void display(void)
{
    int i;
    int j;
    static int r = 0; /* 回転角 */

    /* ウィンドウの塗りつぶし */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    /* 視点方向と視線方向 */
    gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    /* 光源の位置設定 */
    /* 左から　設定する光源の番号，設定するパラメータの種類，指定したパラメータの種類に設定する値 */
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

    /* モデルビュー変換行列の保存 */
    glPushMatrix();

    /* 図形の回転 */
    glRotated((double)r, 0.0, 1.0, 0.0);

    /* 図形の色(赤) */
    /* 左から　材質パラメータを設定する面(表，裏，両面)，材質の特性，1つまたは4つの要素を持つGLfloat型の配列 */
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

    /* 図形の描画 */
    cube();

    /* 2つ目の図形の描画 */

    /* glMatrixMode()で指定している現在の変換行列を保存 */
    glPushMatrix();

    glTranslated(1.0, 1.0, 1.0);
    glRotated((double)(2 * r), 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    cube();
    glPopMatrix();

    /* モデルビュー変換行列の復帰 */

    /* glPopMatrix()で保存した変換行列を復帰する */
    /* glPushMatrix()を呼び出したあと， glTranslated()やglRotated()あるいはgluLookAt()などを使って変換行列を変更しても，これを呼び出すことでそれ以前の変換行列に戻すことができる */
    glPopMatrix();

    /* ダブル・バッファリング(画面を2つに分け，一方を表示している間にもう一方に図形を描き，それが完了したら2つの画面を入れ替える)の2つのバッファを交換する */
    /* glFrush() は自動的に実行される */
    glutSwapBuffers();

    /* 一周回ったら回転角を 0 に戻す */
    if (++r >= 360) r = 0;
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);

    /* 透視変換行列の設定 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* 変換行列に透視変換の行列を乗ずる */
    /* 左から　カメラの画角[deg](大きいほどワイドレンズになり小さいほど望遠レンズになる)，画面のアスペクト比(縦横比)，表示する奥行き方向の範囲(前方面，後方面) */
    /* 画像を歪ませないようにするにはアスペクト比を glViewport() で指定したビューポートの縦横比と一致させる */
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

    /* モデルビュー変換行列の設定 */
    /* 引数が GL_PROJECTION なら透視変換行列を指定 */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            /* アニメーション開始 */
            /* このプログラムが暇な(何もすることがない)ときに実行する関数のポインタを指定 */
            glutIdleFunc(idle);
        }
        else {
            /* アニメーション停止 */
            glutIdleFunc(0);
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            /* コマ送り (1ステップだけ進める) */
            glutPostRedisplay();
        }
        break;
    default:
        break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
        case 'Q':
        case '\033':
            exit(0);
        default:
            break;
    }
}

void init(void)
{
    /* glClear(GL_COLOR_BUFFER_BIT)でウィンドウを塗りつぶす際の色を指定 */
    glClearColor(1.0,1.0,1.0,1.0);

    /* 隠面消去処理 */
    glEnable(GL_DEPTH_TEST);

    /* カリング(見えない面を取り除く) */
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    /* 陰影付け */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
    /* GLUT及びOpenGL環境を初期化 */
    glutInit(&argc, argv);

    /* ディスプレイの表示モードの設定 */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    /* ウィンドウを開く */
    glutCreateWindow(argv[0]);

    /* 開かれたウィンドウ内に描画する関数へのポインタ */
    glutDisplayFunc(display);

    glutReshapeFunc(resize);

    glutMouseFunc(mouse);

    glutKeyboardFunc(keyboard);

    init();

    /* 無限ループ(イベントの待受状態) */
    glutMainLoop();

    return 0;
}