#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#define M_PI 3.14159265358979323846

// Window dimensions
const int width = 800;
const int height = 600;

// Player ball position
float ballX = 30;
float ballY = 30;
float ballRadius = 20.0f;
float speed = 5.0f;
float jumpHeight = 0.0f;//the value used for translation
float jumpAnimationHeight = 0.0f;//standby animation location tracking
float jumpHeightOnSpace=0.0f;//jump location tracking
float jumpSpeed = 0.05f;
bool jumpeStatus =false;//ball in a jump state
bool jumpAnimationStatus=false;//jump position tracking
bool jumpDefaltAnimationStatus =false;//standby animation ball position tracking
//refresh
int refreshMills = 30;
// goal ball position
float ballX1 = 700;
float ballY1 = 550;
float ballRadius1 = 20.0f;

// Wall coordinates (multiple walls can be added here)
float wallX1 = 100.0f;
float wallY1 = 100.0f;
float wallWidth = 100.0f;
float wallHeight = 300.0f;

// End goal ball position
float endBallX =700;
float endBallY = 550;

// Game state flag
bool gameOver = false;

void Timer(int value) {
   glutPostRedisplay();
   glutTimerFunc(refreshMills, Timer, 0);


}

void checkEnd() {
  float distance = sqrt(pow(ballX - endBallX, 2) + pow(ballY - endBallY, 2));
  if (distance < ballRadius * 2) {
    gameOver = true;
    std::cout << "Congratulations! You reached the goal!" << std::endl;
  }
}

void drawFilledCircle(float cx, float cy, float radius, int numSegments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void jumpLogic(){
    //on jump animation
    if(jumpeStatus){
            //logic status of the ball position top/bottom
            if(jumpHeightOnSpace>2){
                jumpAnimationStatus=true;
            }
            if(jumpHeightOnSpace<0){
                jumpAnimationStatus=false;
                jumpeStatus=false;
            }
        //ball position updating respect to the logic state
        if(!jumpAnimationStatus){
            jumpHeightOnSpace +=0.2f;
            jumpHeight +=5.0f;

        }
         if(jumpAnimationStatus){
            jumpHeightOnSpace -=0.2f;
            jumpHeight -=5.0f;

        }
    }
    //normal standby jump animation
    if(!jumpeStatus){
            //logic status of the ball position top/bottom
            if(jumpAnimationHeight>0.5){
                jumpDefaltAnimationStatus=true;
            }
            if(jumpAnimationHeight<0){
                jumpDefaltAnimationStatus=false;
            }
        //ball position updating respect to the logic state
        if(!jumpDefaltAnimationStatus){
            jumpAnimationHeight +=0.1f;
            jumpHeight +=2.1f;
        }
         if(jumpDefaltAnimationStatus){
            jumpAnimationHeight -=0.1f;
            jumpHeight -=2.1f;
        }
    }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
   glLoadIdentity();

  glPushMatrix();
  glTranslatef(0.0f, jumpHeight, 0.0f);
  // Draw player ball (white circle)
  glColor3f(1.0f, 1.0f, 1.0f);
  drawFilledCircle(ballX, ballY, ballRadius, 360);

  glPopMatrix();// restore translations
  // Draw walls (brown rectangles)
  glColor3f(0.6f, 0.4f, 0.2f);
  glBegin(GL_QUADS);
  glVertex2f(wallX1, wallY1);
  glVertex2f(wallX1 + wallWidth, wallY1);
  glVertex2f(wallX1 + wallWidth, wallY1 + wallHeight);
  glVertex2f(wallX1, wallY1 + wallHeight);
  glEnd();


  // Draw end goal ball (green circle)
  if (!gameOver) {
    glColor3f(0.0f, 1.0f, 0.0f);
    drawFilledCircle(ballX1, ballY1, ballRadius1, 360);
    glVertex2f(endBallX, endBallY);

  }
  jumpLogic();// jump logic
  glutSwapBuffers();
  //jumpHeight += 3.0f;

}
void collision(){
      // Check for collision with wall
  if (ballX + ballRadius > wallX1 && ballX - ballRadius < wallX1 + wallWidth &&
      ballY + ballRadius > wallY1 && ballY - ballRadius < wallY1 + wallHeight) {
    // Bounce back from the wall
    if (ballX + ballRadius > wallX1 && ballX - ballRadius < wallX1 + wallWidth / 2.0f) {
      ballX -= speed;
    } else {
      ballX += speed;
    }
  }
}

void moveBall(int key, int x, int y) {

  switch (key) {
    case GLUT_KEY_LEFT:
      if (ballX - ballRadius > 0) {
        ballX -= speed;
        glutPostRedisplay();
      }
      break;
    case GLUT_KEY_RIGHT:
      if (ballX + ballRadius < width) {
        ballX += speed;
        glutPostRedisplay();
      }
      break;
    case GLUT_KEY_UP:
      if (ballY + ballRadius < height) {
        ballY += speed;
        glutPostRedisplay();
      }
      break;
    case GLUT_KEY_DOWN:
      if (ballY - ballRadius > 0) {
        ballY -= speed;
        glutPostRedisplay();
      }
      break;
  }

    //check for collision
    collision();
  // Check if player reached end goal
   checkEnd();

  glutPostRedisplay();
}

void jump(unsigned char key, int, int) {
    if (key == ' ') {
        // Jump action
        jumpeStatus=true;
        glutPostRedisplay();
    }
}

void initialize() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("2D Platformer");
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Set background color (dark gray)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0f, width, 0.0f, height); // Set 2D orthogonal projection
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  initialize();
  glutDisplayFunc(display);
  glutSpecialFunc(moveBall);
  glutKeyboardFunc(jump);
  glutTimerFunc(0, Timer, 0);
  glutMainLoop();
  return 0;
}
