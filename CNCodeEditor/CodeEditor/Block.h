#ifndef _BLOCK_H
#define _BLOCK_H

#include "../Utilities/Array.h"

class Block {
public:
	Block(Long level, Long head, Long tail, Long capacity = 8);
	Block(const Block& source);
	~Block();
	Block& operator=(const Block& source);

	Long Add(Block* child);
	Long Add(Long index, Block* child);
	Long Remove(Long index);
	Block* GetAt(Long index);

	void Close(Long tail, bool isClosed = true);
	Block* Find(Long row); // row가 속한 최하위 레벨 블록을 반환한다. 없으면 자신.
	Block* Find(Long row, Long level); // row가 속한 그리고 원하는 레벨의 블록을 반환한다. 없으면 자신.
	Long FindChild(Long row); // row가 속한 바로 다음 레벨 블록의 위치를 찾아 반환한다. 없으면 -1.

	Block* Clone();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetLevel() const;
	bool GetIsClosed() const;
	Long GetHead() const;
	Long GetTail() const;

private:
	Array<Block*> childs;
	Long capacity;
	Long length;
	Long level;
	bool isClosed;
	Long head;
	Long tail;
};

inline Long Block::GetCapacity() const {
	return this->capacity;
}

inline Long Block::GetLength() const {
	return this->length;
}

inline Long Block::GetLevel() const {
	return this->level;
}

inline bool Block::GetIsClosed() const {
	return this->isClosed;
}

inline Long Block::GetHead() const {
	return this->head;
}

inline Long Block::GetTail() const {
	return this->tail;
}

#endif //_BLOCK_H