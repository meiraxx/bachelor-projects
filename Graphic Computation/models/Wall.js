'use strict';

function addWallStructure(obj, x, y, z, width, height) {

    mesh = new THREE.Mesh( new THREE.BoxGeometry( 10, width, 30 ), material );
    mesh.position.set(x, y, z);
    mesh.rotation.x = Math.PI/2;

    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function createWall(x, y, z, width, height) {

    material = new THREE.MeshPhongMaterial({
            color: WALL_MAT_COLOR,
            specular: WALL_MAT_SPECULAR,
            shininess: WALL_MAT_SHININESS,
            visible: VISIBLE_BONDS,
            shading: THREE.SmoothShading
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            visible: VISIBLE_BONDS,
            color: WALL_MAT_COLOR,
            shading: THREE.SmoothShading
    });
    var baseMat = new THREE.MeshBasicMaterial({
            visible: VISIBLE_BONDS,
            color: WALL_MAT_COLOR
    });

    var current = phongMat;
    var base = false;

    var wall = new THREE.Object3D();
    addWallStructure(wall, 0, 0, 0, width, height);

    wall.userData = {
        oldposition: wall.position,

        BB: null,

        health: 1000,
        damage: 0,
        type: "wall",
        colision_block: true,
        had_collision: false

    };

wall.changeMats = function() {
  if (base == true)
    return;
    
    (current == phongMat) ? current = lambMat : current = phongMat;

  wall.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

wall.baseMat = function() {
    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  wall.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
}

wall.movement = function (time) {
    return;
};

wall.collision = function(obj) {
    return;
};

wall.makeBB = function() {
    wall.userData.BB = new THREE.Box3().setFromObject(wall)
};


    wall.position.set(x, y, z)

    return wall;
}
