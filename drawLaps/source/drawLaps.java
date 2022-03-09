import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class drawLaps extends PApplet {

double[][] coords = new double[1000000][];
int counter = 0;
int tot;
double scaleX, scaleY;
double scale;

public void setup() {
  
  background(255);
  double minX, minY, maxX, maxY;
  String[] lines = loadStrings("coords.txt");
  double firstX = Double.parseDouble(split(lines[0], ' ')[0]);
  double firstY = Double.parseDouble(split(lines[0], ' ')[1]);
  minX = maxX = 0;
  minY = maxY = 0;
  tot = lines.length;
  for (int i = 0; i < lines.length; i++) {
    coords[i] = new double[2];
    coords[i][0] = (Double.parseDouble(split(lines[i], ' ')[0])-firstX);
    coords[i][1] = (Double.parseDouble(split(lines[i], ' ')[1])-firstY);
    if (coords[i][0] < minX)
      minX = coords[i][0];
    if (coords[i][0] > maxX)
      maxX = coords[i][0];
    if (coords[i][1] < minY)
      minY = coords[i][1];
    if (coords[i][1] > maxY)
      maxY = coords[i][1];
  }
  scaleX = (width/2)/(maxX-minX);
  scaleY = (height/2)/(maxY-minY);
  scale = scaleX>scaleY?scaleX:scaleY;
  println(minX, maxX, minY, maxY, height/2, width/2);
  println(scaleX, scaleY);
  frameRate(200);
}

public void draw() {
  if (counter >= tot) {
    frameRate(0);
    return;
  }
  coords[counter][0] *= scale;
  coords[counter][1] *= scale;
  println(coords[counter][0], coords[counter][1]);
  noStroke();
  fill(0, 10);
  translate(width/2, height/2);
  circle((float)coords[counter][0], (float)coords[counter][1], 5);
  counter++;
}
  public void settings() {  size(1000, 1000); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "drawLaps" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
