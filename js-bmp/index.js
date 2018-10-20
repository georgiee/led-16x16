const fs = require('fs');

var bmp = require("bmp-js");
var bmpBuffer = fs.readFileSync('test.bmp');
var bmpData = bmp.decode(bmpBuffer);

console.log(bmpData);

const print = index => {
  console.log(index + ': ', bmpData.data[index + 0], bmpData.data[index + 1], bmpData.data[index + 2], bmpData.data[index + 3]);
};

for(let i = 0; i < 256; i++) {
  print(i);
}