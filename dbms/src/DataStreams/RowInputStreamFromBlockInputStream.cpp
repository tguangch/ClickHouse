#include <DB/DataStreams/RowInputStreamFromBlockInputStream.h>


namespace DB
{

using Poco::SharedPtr;


RowInputStreamFromBlockInputStream::RowInputStreamFromBlockInputStream(BlockInputStreamPtr block_input_)
	: block_input(block_input_), pos(0), current_rows(0)
{
}


bool RowInputStreamFromBlockInputStream::read(Row & row)
{
	if (pos >= current_rows)
	{
		current_block = block_input->read();
		current_rows = current_block.rows();
		pos = 0;
	}

	size_t columns = current_block.columns();
	row.resize(columns);

	for (size_t i = 0; i < columns; ++i)
		row[i] = (*current_block.getByPosition(i).column)[pos];

	++pos;
	return true;
}

}
