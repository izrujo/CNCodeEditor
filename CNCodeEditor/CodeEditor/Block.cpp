#include "Block.h"

Block::Block(Long level, Long head, Long tail, Long capacity)
	: childs(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->level = level;
	this->isClosed = false;
	this->head = head;
	this->tail = tail;
}

Block::Block(const Block& source)
	: childs(source.childs) {
	Long i = 0;
	while (i < source.length) {
		this->childs.Modify(i, (const_cast<Block&>(source)).childs[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->level = source.level;
	this->isClosed = source.isClosed;
	this->head = source.head;
	this->tail = source.tail;
}

Block::~Block() {
	Long i = 0;
	while (i < this->length) {
		if (this->childs[i] != 0) {
			delete this->childs[i];
		}
		i++;
	}
}

Block& Block::operator=(const Block& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->childs[i] != 0) {
			delete this->childs[i];
		}
		i++;
	}
	this->childs = source.childs;
	i = 0;
	while (i < source.length) {
		this->childs.Modify(i, (const_cast<Block&>(source)).childs[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	this->level = source.level;
	this->isClosed = source.isClosed;
	this->head = source.head;
	this->tail = source.tail;

	return *this;
}

Long Block::Add(Block* child) {
	Long index;
	if (this->length < this->capacity) {
		index = this->childs.Store(this->length, child);
	}
	else {
		index = this->childs.AppendFromRear(child);
		this->capacity++;
	}
	this->length++;

	return index;
}

Long Block::Add(Long index, Block* child) {
	index = this->childs.Insert(index, child);
	this->length++;
	this->capacity++;

	return index;
}

Long Block::Remove(Long index) {
	if (this->childs[index] != 0) {
		delete this->childs.GetAt(index);
	}
	index = this->childs.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

Block* Block::GetAt(Long index) {
	return this->childs.GetAt(index);
}

void Block::Close(Long tail, bool isClosed) {
	this->tail = tail;
	this->isClosed = isClosed;
}

Block* Block::Find(Long row) {
	Block* key = this;

	Long index = this->FindChild(row);

	if (index != -1) {
		key = this->childs[index];
		key = key->Find(row);
	}

	return key;
}

Block* Block::Find(Long row, Long level) {
	Block* key = this;

	Long index = this->FindChild(row);

	if (index != -1) {
		key = this->childs[index];
		if (key->GetLevel() < level) {
			key = key->Find(row);
		}
	}

	return key;
}

Long Block::FindChild(Long row) {
	Long index = -1;

	Block* child;
	Long head = -1;
	Long tail = -1;
	Long i = 0;
	while (i < this->length && ((row < head || row >= tail) || i == 0)) {
		child = this->childs[i];
		head = child->GetHead();
		tail = child->GetTail();
		i++;
	}
	if (row >= head && row <= tail) {
		index = i - 1;
	}

	return index;
}

Block* Block::Clone() {
	return new Block(*this);
}