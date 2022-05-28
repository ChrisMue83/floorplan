#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>

using namespace std;

class draw {
  double pi = 3.14159265359;
  bool area_flag = false;

  string ext_string(string input, string codeword) {
    int i = input.find(codeword);
    int j = input.find(";", i);
    return input.substr(i + codeword.size() + 1, j - i - codeword.size() - 1);
  }

public:
  bool showlength, noheader, noending;
  int beginarea, endarea;
  double startx, starty, startangle, minimumx, minimumy, maximumx, maximumy;
  vector<double> f1, f2, xcor, ycor, anglecor;
  vector<string> f3;
  vector<string> point_name;
  vector<int> point_number;

  draw() {
    startx = 0;
    starty = 0;
    startangle = 0, minimumx = 1E15, minimumy = 1E15, maximumx = -1E15,
    maximumy = -1E15;
    showlength = false;
    xcor.push_back(0);
    ycor.push_back(0);
    anglecor.push_back(0);
    noheader = false;
    noending = false;
  }

  void header() {
    cout << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            "xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\""
         << -100 << " " << -100 << " " << maximumx - minimumx + 300 << " "
         << maximumy - minimumy + 300 << "\">" << endl;
  }

  void ending() { cout << "</svg>" << endl; }

  void drawline(double length, double angle, string color, string width,
                bool show) {

    length = length * 100;

    angle = pi / 180 * angle;
    double newx, newy;
    newx = startx + cos(startangle + angle) * length;
    newy = starty + sin(startangle + angle) * length;

    if (color != "blank") {
      if (show == true) {
        cout << "<line x1=\"" << startx - minimumx << "\" y1=\""
             << starty - minimumy << "\"  x2=\"" << newx - minimumx
             << "\"  y2=\"" << newy - minimumy << "\" style=\"stroke:" << color
             << "; stroke-width:" << width << "\"/>" << endl;

        if (showlength == true)
          cout << "<text x=\"" << (startx + newx) * 0.53 - minimumx << "\" y=\""
               << (starty + newy) * 0.53 - minimumy << "\" class=\"smaller\">"
               << length / 100. << " m </text>" << endl;
      }
      xcor.push_back(newx);
      ycor.push_back(newy);
      anglecor.push_back(startangle + angle);
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
    // Greenfunction to calculate the area of a polygon

    double fx, fy, area = 0;
    double averagex = 0, averagey = 0;

    for (int i = start; i < start + number + 1; i++) {

      averagex = averagex + xcor[i];
      averagey = averagey + ycor[i];

      if (i < start + number)
        area += (-(xcor[i + 1] - xcor[i]) *
                     (ycor[i] + 0.5 * (ycor[i + 1] - ycor[i])) +
                 (ycor[i + 1] - ycor[i]) *
                     (xcor[i] + 0.5 * (xcor[i + 1] - xcor[i])));
      else
        area += (-(xcor[start] - xcor[i]) *
                     (ycor[i] + 0.5 * (ycor[start] - ycor[i])) +
                 (ycor[start] - ycor[i]) *
                     (xcor[i] + 0.5 * (xcor[start] - xcor[i])));
    }
    /*
cout << "<polygon points=\"";
for (int i = start; i < start + number + 1; i++) {
  cout << xcor[i] - minimumx << "," << ycor[i] - minimumy << " ";
}
cout << "\" style=\"fill:rgb(250, 250, 250)\" />" << endl;
*/

    area = area * 0.5 / 10000.;
    if (area < 0)
      area = -area;
    averagex = averagex / (number + 1);
    averagey = averagey / (number + 1);
    cout << "<text x=\"" << averagex - minimumx << "\" y=\""
         << averagey - minimumy << "\" class=\"big\">" << area << " m²</text>"
         << endl;
  }

  void reset() {
    startx = 0;
    starty = 0;
    startangle = 0;
    showlength = false;
    xcor.resize(1);
    ycor.resize(1);
    anglecor.resize(1);
  }

  void read(string filename) {
    ifstream infile(filename);

    string line;
    while (std::getline(infile, line)) {
      istringstream iss(line);

      if (line.find("#") == string::npos && !line.empty()) {
        if (line.find("->") != string::npos) {
          f1.push_back(0);
          f2.push_back(0);
          f3.push_back(line);
        } else {
          double a, b;

          if (!(iss >> a >> b)) {
            break;
          }
          f1.push_back(a);
          f2.push_back(b);
          f3.push_back("0");
        }
      }
    }
  }

  void commands(bool show) {

    string color = "black", width = "1";
    vector<string> commandos;
    commandos.push_back("Restart");
    commandos.push_back("Color");
    commandos.push_back("Width");
    commandos.push_back("SavePoint");
    commandos.push_back("GoToPoint");
    commandos.push_back("GoToXY");
    commandos.push_back("SetAngle");
    commandos.push_back("ShowLength");
    commandos.push_back("HideLength");
    commandos.push_back("NoHeader");
    commandos.push_back("NoEnding");
    commandos.push_back("BeginArea");
    commandos.push_back("EndArea");

    for (int j = 0; j < f3.size(); j++) {
      if (f3[j].find("->") != string::npos) {
        for (int i = 0; i < commandos.size(); i++) {
          if (f3[j].find(commandos[i]) != string::npos) {
            string found = commandos[i];

            if (found == "Restart") {
              reset();
            } else if (found == "Color") {

              color = ext_string(f3[j], "Color");
            } else if (found == "Width") {
              width = ext_string(f3[j], "Width");
            } else if (found == "BeginArea") {
              beginarea = xcor.size();
              area_flag = true;
            } else if (found == "EndArea") {
              endarea = xcor.size();

              if (area_flag == false) {
                cout << "Error: BeginArea or EndArea is missing" << endl;
                exit(0);
              }

              if (show == true)
                area(beginarea - 1, endarea - beginarea);
            } else if (found == "SavePoint") {
              point_name.push_back(ext_string(f3[j], "SavePoint"));
              point_number.push_back(xcor.size());

            } else if (found == "GoToPoint") {
              for (int k = 0; k < point_name.size(); k++) {
                if (point_name[k] == ext_string(f3[j], "GoToPoint")) {
                  startx = xcor[point_number[k] - 1];
                  starty = ycor[point_number[k] - 1];
                  startangle = anglecor[point_number[k] - 1];
                  xcor.push_back(startx);
                  ycor.push_back(starty);
                  break;
                }
              }
            } else if (found == "GoToXY") {
              string xy_s = ext_string(f3[j], "GoToXY");
              istringstream iss(xy_s);
              double a, b;
              iss >> a >> b;
              startx = a;
              starty = b;

            } else if (found == "SetAngle") {
              string angle_s = ext_string(f3[j], "SetAngle");
              istringstream iss(angle_s);
              double a;
              iss >> a;
              startangle = a;

            } else if (found == "ShowLength") {
              showlength = true;
            } else if (found == "HideLength") {
              showlength = false;
            } else if (found == "NoHeader") {
              noheader = true;
            } else if (found == "NoEnding") {
              noending = true;
            }
          }
        }

      } else {
        drawline(f1[j], f2[j], color, width, show);
      }
    }
  }
};

int main(int argc, char *argv[]) {

  draw drawobj;
  if (argc != 2)
    exit(0);
  drawobj.read(argv[1]);
  drawobj.commands(false);
  drawobj.reset();
  if (drawobj.noheader == false)
    drawobj.header();

  drawobj.commands(true);
  if (drawobj.noending == false)
    drawobj.ending();
}
