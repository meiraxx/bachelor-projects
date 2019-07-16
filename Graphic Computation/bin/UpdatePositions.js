'use strict';

function spaceshipNextPosition () { // Ler input e preparar next position
    spaceship.userData.speedX = 0; spaceship.userData.rotation = 0;  // Serve para limitar a max speed

    if (spaceship.userData.direction != 0)      // Obter nova posicao + rotacao da nave
        (spaceship.userData.direction == 1) ? leftArrow() : rightArrow();
    else
        iddle();
}

function updatePositions() {

    if (globaltime == 0) return;
    /* Para cada objecto1, ve onde este vai parar, se bater em algum, corre o codigo
    de colisao do objecto1 a receber os n objectos que vai bater, e para esses n objectos
    corre o codigo de colisao a receber o objecto1 */
    for (var i = 0; i < list_objects.length; i++) {
        list_objects[i].movement(globaltime);
        if (checkInitialCollisions(list_objects[i]))
            if (checkRealCollisions(list_objects[i])) {
                for (var j = 0; j < list_collisions.length; j++)
                    list_objects[i].collision( list_collisions[j] );
                for (j = 0; j < list_collisions.length; j++)
                    list_collisions[j].collision( list_objects[i] );

                list_collisions = [];          // Limpar lista temporaria de colisoes
            }
    }

    /* Se algum objecto tem 0 de vida, este deve ser removido
    e aproveita e da reset na variavel had_collision */
    for (i = list_objects.length-1; i >= 0; i--) {
        list_objects[i].userData.had_collision = false;
        if (list_objects[i].userData.health <= 0) {
            if (SHOW_BB)
                scene.remove(list_objects[i].userData.bbhelper);
            if (list_objects[i].userData.type == "alien") {
                spaceship.userData.score++;
                if (spaceship.userData.score == (ALIEN_LINES*ALIEN_COLUMNS)) {
                    gScreen.changeTxt('WIN');
                    gScreen.visible = true;
                }
            }
            if (list_objects[i].userData.type == "player") {
                gScreen.changeTxt('GAME_OVER');
                gScreen.visible = true;
                time.running = false;
            }
            scene.remove(list_objects[i]);
            removeFromGlobalList(list_objects[i])
            list_objects.splice(i,1);
        }
    }
}


function checkInitialCollisions(obj1) {     // Verifica se ha objectos no raio do objecto1
    var distance = 0, sumradius = 0;
    var obj2;
    for (var i = 0; i < list_objects.length; i++) {

        if (obj1.uuid == list_objects[i].uuid)
            continue;

        obj2 = list_objects[i];
        distance = obj1.position.distanceToSquared(obj2.position);                  // Distancia entre os 2 objectos
        sumradius = Math.pow( (obj1.userData.radius + obj2.userData.radius) , 2 );  // Soma dos 2 raaios

        if (distance < sumradius)       // Se os 2 raios sao maiores do que a distancia, entao ha possivel colisao
            list_collisions.push(obj2);   // Meter objectos nas listas de colisao de cada objecto

    }

    /* Ver se ha colisao entre object1 e mundo */
    for (i = 0; i < list_world_obstacles.length; i++)
        if (aabbTester( obj1 , list_world_obstacles[i] ))
            list_collisions.push(list_world_obstacles[i])

    return (list_collisions.length != 0);
}


function checkRealCollisions(obj1) {
    var obj2;
    var list_temp = [];

    /* Ve se na lista de colisoes, algum dos objectos esta a intersectar
    a aabb do obj1. Se tiver, este objecto que intersecta vai ficar na lista de
    colisoes reais */
    for (var i = 0; i < list_collisions.length; i++) {
        obj2 = list_collisions[i];
        if (aabbTester( obj1, obj2) )
            list_temp.push(obj2);
    }

    list_collisions = list_temp;
    return (list_collisions.length != 0);
}


function aabbTester(obj1, obj2) {

    var x1 = (obj1.userData.BB.getSize().x) / 2;
    var z1 = (obj1.userData.BB.getSize().z) / 2;
    var x2 = (obj2.userData.BB.getSize().x) / 2;
    var z2 = (obj2.userData.BB.getSize().z) / 2;
    var pos1 = obj1.position;
    var pos2 = obj2.position;

    if ( ((pos1.x+x1) >= (pos2.x-x2)) && ((pos1.x-x1) <= (pos2.x+x2)) &&
         ((pos1.z+z1) >= (pos2.z-z2)) && ((pos1.z-z1) <= (pos2.z+z2)) )
             return true;

    return false;

}


function updateBBs() {
    for (var i in list_objects)
        list_objects[i].userData.bbhelper.update();

}

function removeFromGlobalList(obj) {
    if (obj.userData.type == "ally")
        return;

    var uuid = obj.uuid;
    for (var i = 0; i < list_ALL.length; i++)
        if (list_ALL[i].uuid == uuid) {
            list_ALL.splice(i, 1);
            return;
        }
}
