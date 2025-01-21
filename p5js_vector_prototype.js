class Vec3 {
  constructor(x, y, z){
    this.x = x;
    this.y = y;
    this.z = z;
  }
  
  mag(){
    return Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
  }
  
  add(v){
    this.x += v.x;
    this.y += v.y;
    this.z += v.z;
  }
  
  static add(a, b){
    return new Vec2(a.x + b.x, a.y + b.y, a.z + b.z);
  }
  
  sub(v) {
    this.x -= v.x;
    this.y -= v.y;
    this.z -= v.z;
  }
  
  static sub(a, b){
    return new Vec2(a.x - b.x, a.y - b.y, a.z - b.z);
  }
  
  dot(v){
    return (this.x * v.x + this.y * v.y + this.z * v.z);
  }
  
  static dot(a, b){
    return (a.x * b.x + a.y * b.y + a.z * b.z);
  }
  
  scale(n){
    this.x *= n;
    this.y *= n;
    this.z *= n;
  }  
  
  cross(v){
    return new Vec3(this.y * v.z - this.z * v.y, this.z * v.x - this.x * v.z, this.x * v.y - this.y * v.x)
  }
  
  static cross(a, b){
    return new Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)
  }
  normalize(){
    let len = this.mag();
    this.x /= len;
    this.y /= len;
    this.z /= len;
  }
  
  static normalize(v){
    v.normalize();
  }
  
}

class Vec2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  
  mag(){
    return Math.sqrt((this.x * this.x) + (this.y * this.y));
  }
  
  add(v){
    this.x += v.x;
    this.y += v.y;
  }
  
  static add(a, b){
    return new Vec2(a.x + b.x, a.y + b.y);
  }
  
  sub(v) {
    this.x -= v.x;
    this.y -= v.y;
  }
  
  static sub(a, b){
    return new Vec2(a.x - b.x, a.y - b.y);
  }
  
  dot(v){
    return (this.x * v.x + this.y * v.y);
  }
  
  static dot(a, b){
    return (a.x * b.x + a.y * b.y);
  }
  
  scale(n){
    this.x *= n;
    this.y *= n;
  }
  
  perpendicular(v){
    return new Vec2(this.y, -this.x)
  }
  
  normalize(){
    let len = this.mag();
    this.x /= len;
    this.y /= len;
  }
  
  static normalize(v){
    v.normalize();
  }
  
  rotate(angle){
    return new Vec2(this.x * Math.cos(angle) - this.y * Math.sin(angle), this.x * Math.sin(angle) + this.y * Math.cos(angle));
  }
  
  static rotate(v, angle){
    v.rotate(angle);
  }
  
  draw(color) {
    fill(color);
    stroke("white");
    line(0, 0, this.x, this.y)
    circle(this.x, this.y, 10);
  }
}

let position = new Vec2(100, 0);
let velocity = new Vec2(1, 2);
let angle = 0.0;

function setup() {
  createCanvas(windowWidth, windowHeight);
}

function draw() {
  translate(windowWidth/2, windowHeight/2);
  background("black");
  let rotatedPosition = position.rotate(angle);
  rotatedPosition.draw("red");
  angle += 0.03;
  //position.x = mouseX;
  //position.y = mouseY;
  //position.add(velocity);
  //position = Vec2.add(position, velocity);
  //position.draw("red");
  //console.log(position.mag());
}