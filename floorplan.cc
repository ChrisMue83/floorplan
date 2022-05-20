#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

class draw {
  double pi = 3.14159265359;

public:
  double startx, starty, startangle, minimumx, minimumy, maximumx, maximumy;
  bool showlength;
  vector<double> f1, f2, xcor, ycor;
  vector<string> f3;
  vector<int> beginarea, endarea;

  draw() {
    startx = 0;
    starty = 0;
    startangle = 0, minimumx = 1E15, minimumy = 1E15, maximumx = -1E15,
    maximumy = -1E15;

    showlength = false;
    xcor.push_back(0);
    ycor.push_back(0);
  }

  void header() {
    cout << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            "xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\""
         << -100 << " " << -100 << " " << maximumx - minimumx + 300 << " "
         << maximumy - minimumy + 300 << "\">" << endl;
  }

  void ending() { cout << "</svg>" << endl; }

  void drawline(double length, double angle, string color, double width,
                bool show) {

    length = length * 100;

    angle = pi / 180 * angle;
    double newx, newy;
    newx = startx + cos(startangle + angle) * length;
    newy = starty + sin(startangle + angle) * length;

    if (show == true) {
      cout << "<line x1=\"" << startx - minimumx << "\" y1=\""
           << starty - minimumy << "\"  x2=\"" << newx - minimumx << "\"  y2=\""
           << newy - minimumy << "\" style=\"stroke:" << color
           << "; stroke-width:" << width << "\"/>" << endl;

      if (showlength == true)
        cout << "<text x=\"" << (startx + newx) * 0.53 - minimumx << "\" y=\""
             << (starty + newy) * 0.53 - minimumy << "\" class=\"smaller\">"
             << length / 100. << " m </text>" << endl;

      xcor.push_back(newx);
      ycor.push_back(newy);
    }

    startx = newx;
    starty = newy;
    startangle = startangle + angle;
    if (minimumx > startx)
      minimumx = startx;
    if (minimumy > starty)
      minimumy = starty;
    if (maximumx < startx)
      maximumx = startx;
    if (maximumy < starty)
      maximumy = starty;
  }

  void area(int start, int number) {
    // Greenfunction to calculate the area of a polyhedron

    double fx, fy, dx, dy, area = 0, t, dt;
    int bins = 50000;
    dt = 1. / bins;
    double averagex = 0, averagey = 0;

    for (int i = start; i < start + number + 1; i++) {

      averagex = averagex + xcor[i];
      averagey = averagey + ycor[i];

      for (int j = 0; j < bins; j++) {
        if (i < start + number) {
          fx = xcor[i] + (xcor[i + 1] - xcor[i]) * t;
          dx = (xcor[i + 1] - xcor[i]) * dt;
          fy = ycor[i] + (ycor[i + 1] - ycor[i]) * t;
          dy = (ycor[i + 1] - ycor[i]) * dt;
        } else {
          fx = xcor[i] + (xcor[start] - xcor[i]) * t;
          dx = (xcor[start] - xcor[i]) * dt;
          fy = ycor[i] + (ycor[start] - ycor[i]) * t;
          dy = (ycor[start] - ycor[i]) * dt;
        }

        area = area + (-fy * dx + fx * dy);
        t = j * dt;
      }
    }

    averagex = averagex / (number + 1);
    averagey = averagey / (number + 1);

    if (area < 0)
      area = -area;

    cout << "<polygon points=\"";

    for (int i = start; i < start + number + 1; i++) {
      cout << xcor[i] - minimumx << "," << ycor[i] - minimumy << " ";
    }
    cout << "\" style=\"fill:rgb(250, 250, 250)\" />" << endl;

    cout << "<text x=\"" << averagex - minimumx << "\" y=\""
         << averagey - minimumy << "\" class=\"small\">" << 0.5 * area / 10000.
         << " m²</text>" << endl;
  }

  void reset() {
    startx = 0;
    starty = 0;
    startangle = 0;
  }

  void read(string filename) {
    ifstream infile(filename);

    string line;
    while (std::getline(infile, line)) {
      istringstream iss(line);
      if (line.find('#') == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("#");
      }

      else if (line.find("Restart") == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("Restart");
      }

      else if (line.find("BeginArea") == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("BeginArea");
      } else if (line.find("EndArea") == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("EndArea");
      }

      else if (line.find("ShowLength") == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("ShowLength");
      } else if (line.find("HideLength") == 0) {
        f1.push_back(0);
        f2.push_back(0);
        f3.push_back("HideLength");
      }

      else {
        double a, b;
        string c;

        if (!(iss >> a >> b >> c)) {
          break;
        }
        f1.push_back(a);
        f2.push_back(b);
        f3.push_back(c);
      }
    }
  }
};

int main(int argc, char *argv[]) {

  draw drawobj;

  if (argc != 2)
    exit(0);
  drawobj.read(argv[1]);

  for (int i = 0; i < drawobj.f1.size(); i++) {
    if (drawobj.f3[i] == "Restart")
      drawobj.reset();
    if (drawobj.f3[i] != "#" && drawobj.f3[i] != "BeginArea" &&
        drawobj.f3[i] != "EndArea" && drawobj.f3[i] != "ShowLength" &&
        drawobj.f3[i] != "HideLength" && drawobj.f3[i] != "Restart")
      drawobj.drawline(drawobj.f1[i], drawobj.f2[i], drawobj.f3[i], 1, false);
  }

  drawobj.reset();

  drawobj.header();

  for (int i = 0; i < drawobj.f1.size(); i++) {
    if (drawobj.f3[i] == "Restart")
      drawobj.reset();
    if (drawobj.f3[i] == "ShowLength")
      drawobj.showlength = true;
    if (drawobj.f3[i] == "HideLength")
      drawobj.showlength = false;

    if (drawobj.f3[i] != "#" && drawobj.f3[i] != "BeginArea" &&
        drawobj.f3[i] != "EndArea" && drawobj.f3[i] != "ShowLength" &&
        drawobj.f3[i] != "HideLength" && drawobj.f3[i] != "Restart")
      drawobj.drawline(drawobj.f1[i], drawobj.f2[i], drawobj.f3[i], 1, true);

    if (drawobj.f3[i] == "BeginArea")
      drawobj.beginarea.push_back(drawobj.xcor.size());
    if (drawobj.f3[i] == "EndArea")
      drawobj.endarea.push_back(drawobj.xcor.size());
  }

  if (drawobj.beginarea.size() != drawobj.endarea.size()) {
    cout << "Error: BeginArea or EndArea is missing" << endl;
    exit(0);
  }

  for (int i = 0; i < drawobj.beginarea.size(); i++) {
    drawobj.area(drawobj.beginarea[i] - 1,
                 drawobj.endarea[i] - drawobj.beginarea[i]);
  }

  drawobj.ending();
}
