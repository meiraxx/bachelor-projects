function wireframize(obj) {    // Alterar entre modelo arames e solido || Avaliacao 1
    'use strict';

    obj.traverse(
        function(node) {
            if (node instanceof THREE.Mesh) {
                    node.material.wireframe = wireframes;
            }
        }
    );

}

function biggestPow2(value) {   // Devolve a maior potencia de 2 menor do que o value
    for (var n = 2; n < value; n *= 2);
    return n/2;
}
