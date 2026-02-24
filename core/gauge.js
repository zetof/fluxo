function Gauge({x, y, size, color, onColor, offColor, gradient, min, max, decimal, val, old, caption, unit}) {
  
  // Set size of gauge to 200px by default if not set in class
  this.size = size || 200;

  // Create canvas
  canvas = document.createElement('canvas');
  canvas.style.left = x + "px";
  canvas.style.top = y + "px";
  canvas.style.position = "absolute";
  canvas.width = this.size;
  canvas.height =  1.15 * this.size;
  body = document.getElementsByTagName("body")[0];
  body.appendChild(canvas);
  this.context = canvas.getContext("2d");
  
  // Set left upper corner
  this.x = x;
  this.y = y;

  // Set colors
  this.color = color || new Color(255, 255, 255);
  this.onColor = onColor || new Color(190, 190, 190);
  this.offColor = offColor || new Color(63, 63, 63);
  this.gradient = gradient || ["darkblue", "deepskyblue", "green", "orange", "red"];
  
  this.computePos();
  this.caption = caption || "";
  this.unit = unit || "";
  this.center = this.size / 2;

  this.min = min || 0;
  this.max = max || 100;
  this.decimal = decimal || false;
  val = val || this.min + (this.max - this.min) / 2;
  old = old || this.min + (this.max - this.min) / 2;

  this.setVals(old, val);

  if(this.caption != ""){
    this.context.fillStyle = this.color.rgb();
    this.context.font = 10 * Math.round(this.size / 100) + "px monospace";
    this.context.textAlign = "center";
    this.context.textBaseline = "top";
    this.context.fillText(this.caption, this.size / 2, 1.03 * this.size);
  }
}

Gauge.prototype.computePos = function() {
  this.oldPos = (this.old - this.min) / (this.max - this.min);
  this.nowPos = (this.val - this.min) / (this.max - this.min);
}

Gauge.prototype.setVals = function(old, val) {
  if(old >= this.min && old <= this.max && val >= this.min && val <= this.max) {
    if(this.decimal) {
      this.old = 1 * old.toFixed(2);
      this.val = 1 * val.toFixed(2);
    }
    else {
      this.old = 1 * Math.round(old);
      this.val = 1 * Math.round(val);
    }
    this.computePos();
    this.draw();
  }
}

Gauge.prototype.drawNeedle = function(pos, color) {
  
  // We first compute the point where the needle should aim
  // Note that we slightly move the center to have a real needle feeling
  angle = - 10 * Math.PI / 8 + 12 * pos * Math.PI / 8;
  cx = this.size / 2 * (1 + .9 * Math.cos(angle));
  cy = this.size / 2 * (1 + .9 * Math.sin(angle));

  // We then draw an arc centered on this point
  // The size of the circle is half the gauge circle size
  // The line width set to the size of the gauge does the trick to draw a needle
  this.context.beginPath();
  this.context.lineWidth = this.center;
  this.context.strokeStyle = color;
  this.context.arc(cx, cy, this.size / 4, angle + Math.PI - .1, angle + Math.PI + .1);
  this.context.stroke();
  this.context.fillRect(this.center - .75 * this.size / 100, this.center - .75 * this.size / 100, 1.5 * this.size / 100, 1.5 * this.size / 100);
}

Gauge.prototype.draw = function() {

  // Clear canvas
  this.context.clearRect(0, 0, this.size, this.size);
  
  // Draw outer and background circles
  this.context.beginPath();
  this.context.lineWidth = 5;
  this.context.strokeStyle = this.color.rgb();
  this.context.fillStyle = this.offColor.rgb();
  this.context.arc(this.center, this.center, (this.size - 5) / 2, 0, 2 * Math.PI);
  this.context.stroke();
  this.context.fill();

  // Draw left half scale gradient
  this.context.lineWidth = .16 * this.size;
  this.context.beginPath();
  var grad = this.context.createLinearGradient(this.center, this.size, 0, this.center);
  grad.addColorStop(.4, this.gradient[0]);
  grad.addColorStop(.7, this.gradient[1]);
  this.context.strokeStyle = grad;
  this.context.arc(this.center , this.center, (this.size - this.size / 5) / 2, - 5 * Math.PI / 4, -Math.PI);  
  this.context.stroke();
  this.context.beginPath();
  var grad = this.context.createLinearGradient(0, this.center, this.center, 0);
  grad.addColorStop(.2, this.gradient[1]);
  grad.addColorStop(.7, this.gradient[2]);
  this.context.strokeStyle = grad;
  this.context.arc(this.center , this.center,(this.size - this.size / 5) / 2, -Math.PI, -Math.PI / 2);  
  this.context.stroke();
  this.context.beginPath();
  var grad = this.context.createLinearGradient(this.center, 0, this.size, this.center);
  grad.addColorStop(.2, this.gradient[2]);
  grad.addColorStop(.7, this.gradient[3]);
  this.context.strokeStyle = grad;
  this.context.arc(this.center , this.center, (this.size - this.size / 5) / 2, -Math.PI / 2, 0);  
  this.context.stroke();
  this.context.beginPath();
  var grad = this.context.createLinearGradient(0, this.center, 0, this.size);
  grad.addColorStop(.2, this.gradient[3]);
  grad.addColorStop(.6, this.gradient[4]);
  this.context.strokeStyle = grad;
  this.context.arc(this.center , this.center, (this.size - this.size / 5) / 2, 0, Math.PI / 4);  
  this.context.stroke();

  // Draw Needles
  this.drawNeedle(this.oldPos, "grey");
  this.drawNeedle(this.nowPos, "red");

  // Draw value with units
  this.context.fillStyle = this.color.rgb();
  this.context.font = 10 * Math.round(this.size / 100) + "px monospace";
  this.context.textAlign = "center";
  this.context.textBaseline = "middle";
  this.context.fillText(this.val + this.unit, this.center, this.size - 25 * this.size / 100); 
}