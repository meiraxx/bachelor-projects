'use strict';

function loadSkyBox(obj) {

        // Load the skybox images and create list of materials
        var materials = [
            createMaterial( 'images/nebula-xpos.png' ), // right
            createMaterial( 'images/nebula-xneg.png' ), // left
            createMaterial( 'images/nebula-ypos.png' ), // top
            createMaterial( 'images/nebula-yneg.png' ), // bottom
            createMaterial( 'images/nebula-zpos.png' ), // back
            createMaterial( 'images/nebula-zneg.png' )  // front
        ];

        // Create a large cube
        var geometry1 = new THREE.BoxGeometry(CAMERA_WIDTH*2, CAMERA_HEIGHT*2, CAMERA_HEIGHT, 1, 1, 1)
        mesh = new THREE.Mesh( geometry1, new THREE.MeshFaceMaterial( materials ) );

        // Set the x scale to be -1, this will turn the cube inside out
        mesh.scale.set(-1,1,1);
        obj.add( mesh );
}

function loadPlane(obj) {

    geometry = new THREE.PlaneGeometry(CAMERA_WIDTH*3, CAMERA_HEIGHT*3);
    material = createMaterial( 'images/nebula-zneg.png');
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, -70, 0);
    mesh.geometry.normalsNeedUpdate = true;
    mesh.rotation.x = Math.PI/2;
    mesh.rotation.y = Math.PI;
    obj.add(mesh);
}

function createMaterial( path ) {
    var loader = new THREE.TextureLoader();
    var texture = loader.load(path);
    texture.minFilter = THREE.LinearFilter;
    texture.magFilter = THREE.LinearFilter;
    var material = new THREE.MeshBasicMaterial( { map: texture, overdraw: 0.5 } );

    return material;
}

function createbackground_screen(x, y, z) {

	var background_screen = new THREE.Object3D();
    (BACKGROUND == 1) ? loadPlane(background_screen) : loadSkyBox(background_screen);

    background_screen.movement = function () {
        vector.subVectors(spaceship.position, background_screen.position);
        background_screen.translateX(vector.x);
    };

    background_screen.position.set(x, y, z);

    return background_screen;
}
