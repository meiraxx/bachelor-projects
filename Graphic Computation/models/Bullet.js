'use strict';

function addBulletShell(obj, x, y, z) {

    mesh = new THREE.Mesh(new THREE.CylinderGeometry( 1, 1, 4, 32 ), material);
    mesh.position.set(x, y, z);
    mesh.rotation.x = Math.PI/2;
    mesh.geometry.normalsNeedUpdate = true;

    obj.add(mesh);
}

function createBullet(x, y, z) {

    material = new THREE.MeshPhongMaterial({
            color: BULLET_MAT_COLOR,
            specular: BULLET_MAT_SPECULAR,
            shininess: BULLET_MAT_SHININESS,
            shading: THREE.SmoothShading
        });
    var phongMat = material;
        phongMat.needsUpdate = true;
    var lambMat = new THREE.MeshLambertMaterial({
            color: BULLET_MAT_COLOR,
            shading: THREE.SmoothShading
    });
    var baseMat = new THREE.MeshBasicMaterial({
            color: BULLET_MAT_COLOR
    });

    var current = phongMat;
    var base = false;


    var bulletShell = new THREE.Object3D();
        addBulletShell(bulletShell, 0, 0, 0);

    var bullet = new THREE.Object3D();
    bullet.add(bulletShell);

    var bbhelper = new THREE.BoundingBoxHelper(bullet, material);
    var model = new THREE.Box3().setFromObject(bullet);
    var radius = Math.sqrt( Math.pow( (model.getSize().x ) / 2, 2) +
                            Math.pow( (model.getSize().z ) / 2, 2));

    bullet.userData = {
        oldposition: new THREE.Vector3,
        speedZ: -BULLET_SPEED,
        available: true,

        radius: radius,
        BB: model,
        bbhelper: bbhelper,

        health: BULLET_HEALTH,
        damage: BULLET_DAMAGE,
        type: "ally",
        colision_block: false,
        had_collision: false

    };

bullet.changeMats = function() {
    if (base == true)
        return;

    (current == phongMat) ? current = lambMat : current = phongMat;

  bullet.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = current;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

bullet.baseMat = function() {

    (base == false) ? mesh = baseMat : mesh = current;
    base = !base;

  bullet.traverse(
    function(node) {
      if (node instanceof THREE.Mesh) {
        node.material = mesh;
        node.material.wireframe = !wireframes;
      }
    }
  );
};

bullet.movement = function (time) {
    bullet.userData.oldposition = bullet.position.clone();
    bullet.translateZ(15000*time*bullet.userData.speedZ);
};

bullet.collision = function(obj) {

    if (obj.userData.type != "player" && obj.userData.type != "ally") {
        bullet.userData.health--;
        obj.userData.health -= bullet.userData.damage;
    }

    if (obj.userData.type == "wall")
        bullet.userData.health = 0;

    if (bullet.userData.health <= 0)
        bullet.userData.available = true;

};


bullet.reset = function (vector) {
    bullet.position.set(vector.x, vector.y, vector.z);
    bullet.userData.health = BULLET_HEALTH;
    bullet.userData.available = false;
};

    bullet.position.set(x, y, z);

    return bullet;
}
