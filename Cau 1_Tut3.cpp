// Cau1.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <Windows.h>
#include <glut.h>
#include <math.h>

using namespace std;

#define PI			3.1415926
#define DEGTORAD	(PI/180.0)

int		nWidth  = 600;
int		nHeight = 600;

int		nCenterX = nWidth/2;
int		nCenterY = nHeight/2;

int		nRadius = 200;

float	vertexArr0[10][2];
float	vertexArr1[10][2];

float	t = 1;

float   color[3] = {1, 0, 0};

bool	bAuto = false;


void setWindow(float l, float r, float b, float t)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
}
void setViewport(int l, int r, int b, int t)
{
	glViewport(l, b, r-l, t -b);
}

void CalcVertexArrs()
{
	float fAngle = 90;
	float fInc = 360.0/10.0;

	for(int i = 0; i<10; i++)
	{
			//Lần lượt xác định tọa đọ cho các đỉnh của đa giác đều 10 cạnh
			vertexArr0[i][0] = nCenterX + nRadius*cos(fAngle*DEGTORAD);
			vertexArr0[i][1] = nCenterY + nRadius*sin(fAngle*DEGTORAD);

			//Lần lượt xác định tọa độ 5 đỉnh trên cánh của của ngôi sao 5 cánh
			if(i%2 == 0)
			{
				vertexArr1[i][0] = nCenterX + nRadius*cos(fAngle*DEGTORAD);
				vertexArr1[i][1] = nCenterY + nRadius*sin(fAngle*DEGTORAD);
			}
			//Lần lượt xác định tạo độ 5 nút giao của ngôi sao 5 cánh 
			// 5 Nút này thuộc đường tròn có bán kính nhỏ hơn r = nRadius*(sin(fInc*DEGTORAD/2.0)/cos(fInc*DEGTORAD)), dùng tam giác dồng dạng 
			else
			{
				vertexArr1[i][0] = nCenterX + (nRadius*(sin(fInc*DEGTORAD/2.0)/cos(fInc*DEGTORAD)))*cos(fAngle*DEGTORAD);
				vertexArr1[i][1] = nCenterY + (nRadius*(sin(fInc*DEGTORAD/2.0)/cos(fInc*DEGTORAD)))*sin(fAngle*DEGTORAD);
			}
		fAngle +=fInc;
	}
}

void DrawFigure(float t) {
	glLineWidth(3);
	glColor3fv(color);
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i<10; i++)
		{
			glVertex2f(vertexArr0[i][0]*(1 - t) + vertexArr1[i][0]*t,
				       vertexArr0[i][1]*(1 - t) + vertexArr1[i][1]*t);
		}
	glEnd();
}

void OnDisplay()
{
	CalcVertexArrs();

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	setViewport(0, nWidth, 0, nHeight);
	setWindow(0, nWidth, 0, nHeight);

	DrawFigure(t);

	glFlush();
}

void OnSpecialKey(int key, int x, int y) 
{
	switch(key) 
	{
	case GLUT_KEY_LEFT:	// Nhấn (<-), dời tâm sang trái 5 pixels
		nCenterX -=  5;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //Nhấn (->), dời tâm sang phải 5 pixels
		nCenterX += 5;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP: // Nhấn mũi tên lên, dời tâm lên trên 5 pixels
		nCenterY += 5;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: // Nhấn mũi tên xuống, dời tâm xuống 5 pixels
		nCenterY -= 5;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_UP: // Nhấn phím Page UP tăng kích thước hình
		nRadius += 1;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN: // Nhấn phím Page Down giảm kich thước hình
		nRadius -= 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void OnTimer(int value)
{
	//Định nghĩa trước 2 kiểu chuyển hình:
	//							+ Từ sao 5 cánh sang đa giác đều 10 cạnh (STar_To_Poly)
	//							+ Từ sao đa giác đều 10 cạnh sang sao 5 cánh (Poly_To_Star)
	static enum Dir{Star_To_Poly, Poly_To_Star};

	//Kiểu chuyển hình đang dùng, mặc định là chuyển từ sao năm cánh sang hình đa giác đều 10 cạnh
	static int dir = Star_To_Poly;
	
	//Thực hiện hiệu ứng nếu bAuto được bật true
	if(bAuto)
	{
			//Nếu đang chuyển từ sao 5 cánh sang đa giác đêu 10 cạnh thì mặc định là t giảm trừ khi t < 0 
			if(dir ==  Star_To_Poly)
			{
					if( t < 0)
						t += 0.05;
					else 

						t -=0.05;
			}
			//Nếu đang chuyển từ sao 5 cánh sang đa giác đêu 10 cạnh thì mặc định là t tăng trừ khi t > 
			else if(dir == Poly_To_Star)
			{				
					if ( t>1)
						t -= 0.05;
					else
						t += 0.05;
			}
		
		//Khi hiệu ứng đạt hình sao năm cánh thì bắt đầu nở ra thành hình đa giác đều 10 cạnh
		if(t > 1)
		{
				dir = Star_To_Poly;

				//Lập tức đảo chuyển động
				t -= 0.05;
				glutPostRedisplay();
				
				glutTimerFunc(0, OnTimer, dir);
		}
		//Khi hiệu ứng đạt hình đa giác 10 cạnh đều thì bắt đầu thu về hình 5 năm cánh
		else if(t < 0)
		{ 
				dir = Poly_To_Star;

				//Lập tức đảo chuyển động
				t +=0.05;
				glutPostRedisplay();

				glutTimerFunc(0, OnTimer, dir);
		}
		//Tiếp tục chạy hiệu ứng như trước đó nếu không có gì đặc biệt
		else
		{
			glutTimerFunc(1000, OnTimer, dir);
			glutPostRedisplay();
		}
		
	}
}
void OnKey(unsigned char key, int x, int y) 
{
	switch(key)
	{
	case 'i':	case 'I': // Bấm phím i/I để chuyển hình sang hình sao 5 (tăng t tới 1) 
		if( t<1 )
			{
			t += 0.05;
			glutPostRedisplay();
			}	
		break; 
	case 'd':	case 'D': // Bấm phím d/D để chuyển hình sang hình đa giác đều 10 cạnh ( giảm t về 0) 
		if( t>0 )
		{
			t -= 0.05;
			glutPostRedisplay();
		}
		break;
	case 'r':	case 'R':  // Chuyển hình sang màu đỏ
		color[0] =1; color[1]=0; color[2] = 0;
		glutPostRedisplay();
		break;
	case 'g':	case 'G': // Chuyển hình sang màu lục
		color[0] =0; color[1]=1; color[2] = 0;
		glutPostRedisplay();
		break;
	case 'b':	case 'B':  // Chuyển hình sang màu lam
		color[0] =0; color[1]=0; color[2] = 1;
		glutPostRedisplay();
		break;
	case 'a':	case 'A': // Tắt mở chế độ hiệu ứng tự động
		bAuto = !bAuto;
		glutTimerFunc(1000, OnTimer, 0);
		break; 
	default:
		break;
	}
}
void OnReshape(int w, int h)
{
	nWidth = w;
	nHeight = h;
}
void OnMouseClick(int button, int state, int x, int y)
{
	// Dịch tâm của hình tới vị trí click chuột trái
	if(state == GLUT_DOWN && button  == GLUT_LEFT_BUTTON) 
		nCenterX = x;
		nCenterY = nHeight - y; //
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	cout << "Click mouse to change the Center of the Figure" << endl;
	cout << "Press Left Arrow to move the Figure to the left" << endl;
	cout << "Press Right Arrow to move the Figure to the right" << endl;
	cout << "Press Up Arrow to move the Figure up" << endl;
	cout << "Press Down Arrow to move the Figure down" << endl;
	cout << "Press Page Up to increase Radius of the Figure" << endl;
	cout << "Press Page Down to decrease Radius of the Figure" << endl;
	cout << "Press i/I to increase the value of t" << endl;
	cout << "Press d/D to decrease the value of t" << endl;
	cout << "Press r/R to change color of Figure to Red" << endl;
	cout << "Press g/G to change color of Figure to Green" << endl;
	cout << "Press b/B to change color of Figure to Blue" << endl;
	cout << "Press a/A to toggle the value of bAuto" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(nWidth, nHeight);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Cau 1 - Tut 3");

	glutSpecialFunc(OnSpecialKey);
	glutKeyboardFunc(OnKey);
	glutReshapeFunc(OnReshape);
	glutMouseFunc(OnMouseClick);
	glutTimerFunc(1000, OnTimer, 0);
	glutDisplayFunc(OnDisplay);
	
	glutMainLoop();
	return 0;
}
