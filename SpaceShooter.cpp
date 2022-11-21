#define GL_SILENCE_DEPRECATION
#include <GLUT/GLUT.h>
#include <cstdlib>
#include <ctime>
#include <iostream>



int colorPlayer1=1;
int colorPlayer0=0;
int speX = 0;
int speY = 0;
int enemyX = 0;
int enemyXDelta = 2;
int enemyY = 0;
int healthBarX = 0;
int enemyBulletY = 0;
int playerBulletY = 0;
int playerLives = 3;
int playerXCurrent=0;
int playerYCurrent=0;
int enemyXCurrent=0;
int enemyYCurrent=0;
int playerBulletYInit=0;
int playerBulletXInit=0;
int enemyBulletXInit=0;
int enemyBulletYInit=0;
int factorX=0;
int factorY=0;

GLboolean bulletsFlag=false;
GLboolean gameOverFlag=false;
GLboolean healthEnemyFlag=false;
GLboolean powerupInitFlag=true;
GLboolean protectFlag=false;
GLboolean healthPlayerFlag=false;
GLboolean displayPowerupFlag=false;
GLboolean overlapPowerupFlag=false;

void displayBulletsEnemy() {
    if (enemyBulletY<=0){
        enemyBulletY=730;
        enemyXCurrent=enemyX;
        healthPlayerFlag=true;
    }
    enemyBulletXInit=enemyXCurrent+50;
    glPushMatrix();
    glBegin(GL_POINTS);
        glVertex2f(enemyBulletXInit , enemyBulletY);
    glEnd();
    glPopMatrix();
    enemyBulletY-=3;
    

}
void displayPowerup(){
    if (displayPowerupFlag==true && overlapPowerupFlag==false){
    if (powerupInitFlag==true){
        factorX = (rand()%700)+50;
        factorY = rand()%650;
        powerupInitFlag=false;
    }
    glPushMatrix();
    glColor3f(0 ,1 ,0);
    glBegin(GL_POINTS);
        glVertex2f(factorX, factorY);
    glEnd();
    glPopMatrix();
    }
}

void displayBulletsPlayer() {
    if (bulletsFlag==true){
        playerBulletXInit= 400+ playerXCurrent;
        playerBulletYInit= 400+ playerYCurrent+20;
        glPushMatrix();
        glTranslated(0, playerBulletY, 0);
        glBegin(GL_POINTS);
            glVertex2f(playerBulletXInit, playerBulletYInit);
        glEnd();
        glPopMatrix();
        playerBulletY+=2;
    }
    if (playerBulletY+playerBulletYInit>=760){
        bulletsFlag=false;
        playerBulletY=0;
    }
}

void displayEnemyHealthBar() {
    glColor3f(1 ,0 ,0);
    glPushMatrix();
    glBegin(GL_POLYGON);
        glVertex2f(0 ,800);
        glVertex2f(0 ,780);
        glVertex2f(800-healthBarX ,780);
        glVertex2f(800-healthBarX, 800);
    glEnd();
    glPopMatrix();
}

void displayEnemy(){
    glPushMatrix();
    glTranslated(enemyX, 0, 0);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
        glVertex2f(20 ,750);
        glVertex2f(20 ,710);
        glVertex2f(160, 710);
        glVertex2f(160 ,750);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(20 ,710);
        glVertex2f(80 ,710);
        glVertex2f(20 ,650);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(100 ,710);
        glVertex2f(160 ,710);
        glVertex2f(160 ,650);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(90, 710);
        glVertex2f(90, 650);
    glEnd();
    enemyX+=enemyXDelta;
    if (enemyX>=650 || enemyX<=0)
        enemyXDelta= -enemyXDelta;
    glPopMatrix();
}
void time(int val)
{
    displayPowerupFlag = (!displayPowerupFlag);
    if (powerupInitFlag==false)
        powerupInitFlag=true;
    colorPlayer1=1;
    colorPlayer0=0;
    protectFlag=false;
    overlapPowerupFlag=false;
    glutPostRedisplay();
    glutTimerFunc(4000,time,0);
}

void displayPlayer(){
    glPushMatrix();
    glColor3f(colorPlayer0, colorPlayer1, colorPlayer1);
    glTranslated(speX, speY, 0);
    glBegin(GL_POLYGON);
        glVertex2f(365 ,410);
        glVertex2f(365 ,390);
        glVertex2f(435, 390);
        glVertex2f(435 ,410);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(365, 410);
        glVertex2f(365, 440);
        glVertex2f(395, 410);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(435, 410);
        glVertex2f(435, 440);
        glVertex2f(405, 410);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(400, 410);
        glVertex2f(400, 440);
    glEnd();
    glPopMatrix();

}

void loseLivesPlayer(){
    if ((healthPlayerFlag==true) && (protectFlag == false) && (enemyBulletY>= 390+speY) && (enemyBulletY<=440+speY) && (enemyBulletXInit>= 365+speX) && (enemyBulletXInit<= 435+speX)){
        playerLives--;
        healthPlayerFlag=false;
    }
    
}

void loseLivesEnemy(){
    if ((healthEnemyFlag == true) && (playerBulletY+playerBulletYInit >= 650) && (playerBulletXInit >= enemyX+20) && (playerBulletXInit <= enemyX+160)){
        healthBarX+=100;
        healthEnemyFlag = false;
        bulletsFlag = false;
    }
}
void powerupPower(){
    if ((factorY>= 390+speY) && (factorY<=440+speY) && (factorX>= 365+speX) && (factorX<= 435+speX)) {
    overlapPowerupFlag=true;
    colorPlayer1=0;
    colorPlayer0=1;
    protectFlag=true;
    }
}

void print(int x, int y, char* string){
    int len, i;

    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    //get the length of the string to display
    len = (int)strlen(string);

    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void checkForGameOver(){
    if (healthBarX >=800 || playerLives <=0)
        gameOverFlag=true;
}

void displayTextLives() {
    glColor3f(1, 0, 0);
    char* p0s[20];
    sprintf((char*)p0s, "Lives Left: %d", playerLives);
    print(650, 760, (char*)p0s);
}

void displayGameOver(){
    glColor3f(1, 0, 0);
    char* p0s[20];
    sprintf((char*)p0s, "Game Over");
    print(400, 400, (char*)p0s);
}
void displayInstructions(){
    glColor3f(1, 0, 0);
    char* p0s[20];
    sprintf((char*)p0s, "Arrow keys to move, 'z' to shoot");
    print(500, 100, (char*)p0s);
}
void Display() {
    if (gameOverFlag==false){
        displayEnemy();
        displayPlayer();
        displayPowerup();
        displayEnemyHealthBar();
        displayBulletsPlayer();
        displayBulletsEnemy();
        displayTextLives();
        checkForGameOver();
        loseLivesEnemy();
        loseLivesPlayer();
        powerupPower();
        displayInstructions();
        glFlush();
        glutPostRedisplay();
        glClear(GL_COLOR_BUFFER_BIT);
 
    }
    else {
        displayGameOver();
        glFlush();
        glutPostRedisplay();
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void spe(int k, int x, int y){
    if (k == GLUT_KEY_RIGHT)
            speX += 10;
    if (k == GLUT_KEY_LEFT)
            speX -= 10;
    if (k == GLUT_KEY_UP)
            speY += 10;
    if (k == GLUT_KEY_DOWN)
            speY -= 10;
    if (k == 'z') {
        bulletsFlag=true;
        playerXCurrent=speX;
        playerYCurrent=speY;
        healthEnemyFlag=true;
        playerBulletY=0;
    }
}

void actM(int button, int state, int x, int y){
   // if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 
}





int main(int argc, char** argr) {
    glutInit(&argc, argr);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Control");
    glutTimerFunc(0,time,0);
    glutDisplayFunc(Display);
    glutMouseFunc(actM);
    glutSpecialFunc(spe);
    glPointSize(20);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gluOrtho2D(0.0, 800, 0.0, 800);

    glutMainLoop();//don't call any method after this line as it will not be reached.
    return 0;
}
