function inToMM(in) = in * 25.4;

holeToHole = inToMM(2.75);
length = 75;
holeDiam = 2;
totalHeight = 70;
height = 5;
overhang = 3;
boardDepth = 5;
boardHeight = 1.75;
supportWidth = 6;

module mount() {
    difference() {
        cube([length + 2*overhang, boardDepth + 5, height]);
        translate([overhang*2, 0, 0]) cube([length - 2*overhang, boardDepth, height]);
        translate([overhang,0,height/2]) cube([length, boardDepth, boardHeight]);
        holeOffset = (length - holeToHole)/2;
        translate([holeOffset + overhang, holeDiam + 1,0]) cylinder(height, holeDiam/2, holeDiam/2);
        translate([holeOffset + overhang + holeToHole, holeDiam + 1,0]) cylinder(height, holeDiam/2, holeDiam/2);
   }
}

module verticalHole(radius, height) {
    rotate([90, 0, 0]) cylinder(height, radius, radius);
}

module support() {
    difference() {
        translate([0, boardDepth + 2, 0]) cube([supportWidth, 3, totalHeight]);
        for (i=[1:5]) {
            offset = totalHeight - i * inToMM(.25);
            translate([0,0,offset]) verticalHole(2, 3);
        }
    }
}

// mount();
midpoint = (length + 2*overhang)/2 - supportWidth/2;

// translate([midpoint - 20, 0, 0]) support();
// translate([midpoint + 20, 0, 0]) support();

support();