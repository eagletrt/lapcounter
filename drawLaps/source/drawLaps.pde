double[][] coords = new double[100000][];

void setup() {
  size(1000, 1000);
  background(255);
  translate(width/2, height/2);
  String[] lines = loadStrings("coords.txt");
  String[] scales = loadStrings("scale.txt");
  float scale = Float.parseFloat(scales[0]);
  double firstX = Double.parseDouble(split(lines[0], ' ')[0]);
  double firstY = Double.parseDouble(split(lines[0], ' ')[1]);
  for (int i = 0; i < lines.length; i++) {
    //println(split(lines[i], ' ')[0]+" "+split(lines[i], ' ')[1]);
    coords[i] = new double[2];
    coords[i][0] = (Double.parseDouble(split(lines[i], ' ')[0])-firstX)*scale;
    coords[i][1] = (Double.parseDouble(split(lines[i], ' ')[1])-firstY)*scale;
    println(coords[i][0], coords[i][1]);
    noStroke();
    fill(0, 100);
    circle((float)coords[i][0], (float)coords[i][1], 5);
  }
  println(scale);
}
