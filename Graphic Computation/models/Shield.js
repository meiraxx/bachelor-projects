'use strict';

function addCube(obj, x, y, z) {

    var geometry1 = new THREE.BoxGeometry(6, 10, 6);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    mesh.scale.y = 3;
    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function addLine(obj, x1, x2, y, z) {

    for(var i=x1; i<=x2; i=i+6) {
           addCube(obj, i, 0, z);
    }
}

function createShield(x, y, z) {

    material = new THREE.MeshPhongMaterial({
            color: SHIELD_MAT_COLOR,
            specular: SHIELD_MAT_SPECULAR,
            shininess: SHIELD_MAT_SHININESS,
            shading: THREE.SmoothShading
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            color: SHIELD_MAT_COLOR,
            shading: THREE.SmoothShading
    });
    var baseMat = new THREE.MeshBasicMaterial({
            color: SHIELD_MAT_COLOR
    });

    var current = phongMat;
    var base = false;

	var shield = new THREE.Object3D();

    addLine(shield, -30, 30, 0, -42);
    addLine(shield, -36, 36, 0, -36);
    addLine(shield, -42, 42, 0, -30);
    addLine(shield, -48, 48, 0, -24);
    addLine(shield, -54, 54, 0, -18);
    addLine(shield, -54, 54, 0, -12);
    addLine(shield, -54, 54, 0, -6);
    addLine(shield, -54, 54, 0, 0);
    addLine(shield, -54, 54, 0, 6);
    addLine(shield, -54, 54, 0, 12);
    addLine(shield, -54, 54, 0, 18);
    addLine(shield, -54, 54, 0, 24);

    addLine(shield, -54, -30, 0, 30);
    addLine(shield, 30, 54, 0, 30);
    addLine(shield, -54, -36, 0, 36);
    addLine(shield, 36, 54, 0, 36);
    addLine(shield, -54, -42, 0, 42);
    addLine(shield, 42, 54, 0, 42);
    addLine(shield, -54, -42, 0, 48);
    addLine(shield, 42, 54, 0, 48);


    var bbhelper = new THREE.BoundingBoxHelper(shield, material);
    var model = new THREE.Box3().setFromObject(shield);
    var radius = Math.sqrt( Math.pow( (model.getSize().x ) / 2, 2) +
                            Math.pow( (model.getSize().z ) / 2, 2));


    shield.userData = {
        oldposition: shield.position,

        radius: radius,
        BB: model,
        bbhelper: bbhelper,

        health: SHIELD_HEALTH,
        damage: SHIELD_DAMAGE,
        type: "field",
        colision_block: true,
        had_collision: false

    };

shield.changeMats = function() {
  if (base == true)
    return;
    
    (current == phongMat) ? current = lambMat : current = phongMat;

  shield.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

shield.baseMat = function() {
    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  shield.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
}

shield.movement = function (time) {
    return;
};

shield.collision = function(obj) {
        shield.userData.health -= obj.userData.damage;
};

    shield.position.set(x, y, z);

    return shield;
}
