#include <string>
#include "targets/body_args_size.h"
#include "ast/all.h"  // all.h is automatically generated

void xpl::body_args_size::do_fundef_node(xpl::fundef_node * const node, int lvl) {
	_allocSize += node->type()->size();									// special variable size
	node->body()->accept(this, lvl);
}

void xpl::body_args_size::do_body_node(xpl::body_node * const node, int lvl) {
	node->blockNode()->accept(this, lvl);
}

void xpl::body_args_size::do_block_node(xpl::block_node * const node, int lvl) {
	if(node->declarations())
		node->declarations()->accept(this, lvl);
	if(node->instructions())
		node->instructions()->accept(this, lvl);
}

void xpl::body_args_size::do_sequence_node(cdk::sequence_node * const node, int lvl) {
	for(size_t i = 0; i < node->size(); i++){
		node->node(i)->accept(this, lvl);
	}
}

void xpl::body_args_size::do_vardecl_node(xpl::vardecl_node * const node, int lvl) {
	_allocSize += node->type()->size();								// alloc size = to each variable's size
}

// every possible sequence node inside a function definition
void xpl::body_args_size::do_if_node(xpl::if_node * const node, int lvl) {
	node->block()->accept(this, lvl);
}

void xpl::body_args_size::do_if_else_node(xpl::if_else_node * const node, int lvl) {
	node->thenblock()->accept(this, lvl);	
	node->elseblock()->accept(this, lvl);	
}

void xpl::body_args_size::do_while_node(xpl::while_node * const node, int lvl) {
	node->block()->accept(this, lvl);
}

void xpl::body_args_size::do_sweep_node(xpl::sweep_node * const node, int lvl) {
	node->block()->accept(this, lvl);
}