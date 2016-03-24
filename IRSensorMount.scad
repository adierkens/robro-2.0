function inToMM(in) = in * 25.4;

boardLength = 76;
boardWidth = 10;
boardHeight = 1.75;
skirt = 1.5;
bottomSkirt = 2;
mountHoleRadius = 1.75;
skirtHeight = 4;
skirtWidth = 2;
totalLength = boardLength + 15;
midpoint = totalLength/2;

linkHeight = 65;
linkWidth = 40;
linkDepth = 5;
linkHoleOffset = 2;

module triangle(o_len, a_len, depth) {
    linear_extrude(height=depth) {
        polygon(points=[[0,0],[a_len,0],[0,o_len]], paths=[[0,1,2]]);
    }
}


module link() {
    difference() {
        cube([linkWidth, linkDepth, linkHeight]);
        translate([7, 0, linkHeight-7]) rotate([-90, 0, 0]) triangle(linkHeight - (7 * 2.5), linkWidth - (7 * 2.5), linkDepth);
        translate([linkWidth - 7, 0, 7]) rotate([-90, -180, 0]) triangle(linkHeight - (7 * 2.5), linkWidth - (7 * 2.5), linkDepth);
        
        for (i=[0:1]) {
            for (n=[0:5]) {
                translate([3.5 + i*(linkWidth - 7), 0, (linkHeight - linkHoleOffset - mountHoleRadius) - (n * (mountHoleRadius + linkHoleOffset + 0.5))]) rotate([-90, 0, 0]) cylinder(linkDepth, mountHoleRadius, mountHoleRadius);
            }
        }
    }
}

module base() {
    
    translate([0, 0, skirtHeight])
    difference() {
        cube([totalLength, 10, boardHeight + 2*bottomSkirt]);
        translate([midpoint - (boardLength/2), 0, skirt]) cube([boardLength, skirt, boardHeight]);        
        for (i=[0:1]) {
            offset = 4;
            translate([offset + i * (totalLength - (offset*(i+1))),0,boardHeight/2 + bottomSkirt]) rotate([-90,0,0]) cylinder(boardWidth + 5, mountHoleRadius, mountHoleRadius);
        }
    }
    
    cube([skirtWidth, 10, skirtHeight]);
    translate([totalLength - skirtWidth, 0, 0]) cube([skirtWidth, 10, skirtHeight]);
    translate([0, 10-skirtWidth, 0]) cube([totalLength, skirtWidth, skirtHeight]);
}

module mount() {
    base();
    translate([midpoint - linkWidth/2, 10-linkDepth, boardHeight + 2*bottomSkirt]) link();
}

mount();

