'use strict';

function addScreenCube(obj, x, y, z) {

    var geometry = new THREE.PlaneGeometry(1, 1);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    mesh.scale.x = MSG_X;
    mesh.scale.y = MSG_Y;
    mesh.geometry.normalsNeedUpdate = true;
    obj.add(mesh);
}

function createScreen(x, y, z) {

    var loader = new THREE.TextureLoader();
	  var texture = loader.load("images/GAME_OVER.png");
    texture.minFilter = THREE.LinearFilter;
    texture.magFilter = THREE.LinearFilter;
    material = new THREE.MeshBasicMaterial({
            map: texture,
            transparent: true
    });

	var screen = new THREE.Object3D();

    addScreenCube(screen, 0, 0, 0);

    screen.rotation.x = -Math.PI/2;
    screen.position.set(x,y,z);

    screen.userData = {
        txt: 'GAME_OVER',
    };

screen.changeTxt = function(img) {
  var newTxt = loader.load('images/' + img + '.png');
  newTxt.minFilter = THREE.LinearFilter;
  newTxt.magFilter = THREE.LinearFilter;
  screen.children[0].material.map = newTxt;
  screen.userData.txt = img;
  };


    screen.position.set(x, y, z);
    return screen;
}
