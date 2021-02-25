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
	Block* Find(Long row); // row�� ���� ������ ���� ����� ��ȯ�Ѵ�. ������ �ڽ�.
	Block* Find(Long row, Long level); // row�� ���� �׸��� ���ϴ� ������ ����� ��ȯ�Ѵ�. ������ �ڽ�.
	Long FindChild(Long row); // row�� ���� �ٷ� ���� ���� ����� ��ġ�� ã�� ��ȯ�Ѵ�. ������ -1.

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