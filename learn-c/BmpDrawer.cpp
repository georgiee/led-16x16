#include "BmpDrawer.h"
#include<string>
#include<iostream>
using namespace std;
// void setFile(SdFile file) {
//   Serial.print("sd file open");
// }

void BmpDrawer::test()
{
  cout << "Works!";
}
void BmpDrawer::setFile(string file)
{
  cout << file;
}

int main()
{
  BmpDrawer drawer;
  drawer.setFile("xxo");
  drawer.test();
  return 0;
}
