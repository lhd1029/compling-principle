#include "Declaration.h"
#include "File.h"
#include <iostream>

void read_file()
{
	has_read = 0;
	if (buffer_forward == 0)
	{
		for (int j = 0; j < left_eof; j++)
		{
			buffer[j] = in_file.get();
		}

	}
	else if (buffer_forward == left_eof)
	{
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			temp_buffer[i] = buffer[i + BUFFER_SIZE_HALF + 1];
			buffer[i + BUFFER_SIZE_HALF + 1] = in_file.get();
			if (in_file.eof())
			{
				has_read = i;
			}
		}
		buffer_forward = left_eof + 1;
	}
	else if (buffer_forward == right_eof)
	{
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			temp_buffer[i] = buffer[i];
			buffer[i] = in_file.get();
			if (in_file.eof())
			{
				has_read = i;
			}
		}

		buffer_forward = 0;
	}
}

void get_char()
{
	C = buffer[buffer_forward];
	buffer_forward++;
	if (buffer_forward == left_eof || buffer_forward == right_eof)
	{
		read_file();
	}
	if (C == '\n')
	{
		line_num++;
	}
	if ((C != '\n') && (C != ' ') && (C != '\t') && (C != EOF) && (C < 128 && C >= 0))
	{
		ch_num++;
	}
}

void retract()
{
	if (buffer_forward == 0)
	{
		buffer_forward = right_eof - 1;
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			buffer[i] = temp_buffer[i];
		}
		if (in_file.eof())
		{
			in_file.close();
			in_file.open("jobshop.c", ios::in);
			in_file.seekg(-has_read, ios::end);
		}
		else
		{
			in_file.seekg(-BUFFER_SIZE_HALF, ios::cur);
		}

	}
	else if (buffer_forward == left_eof + 1)
	{
		buffer_forward = left_eof - 1;
		for (int i = 0; i < BUFFER_SIZE_HALF; i++)
		{
			buffer[i + left_eof + 1] = temp_buffer[i];
		}
		if (in_file.eof())
		{
			in_file.close();
			in_file.open("jobshop.c", ios::in);
			in_file.seekg(-has_read, ios::end);
		}
		else
		{
			in_file.seekg(-BUFFER_SIZE_HALF, ios::cur);
		}
	}
	else
	{
		buffer_forward--;
	}
	if ((C != '\n') && (C != ' ') && (C != '\t'))
	{
		ch_num--;
	}
	if (C == '\n')
	{
		line_num--;
	}
}

void get_nbc()
{
	while (C == ' ' || C == '\n' || C == '\t')
	{
		get_char();
	}
}

void write_file()
{
	for (unsigned int i = 0; i < output_list.size(); i++)
	{
		out_file << output_list.at(i).first << " " << output_list.at(i).second << endl;
	}
	out_file << "The state line number is: " << line_num << endl;
	out_file << "The character number is: " << ch_num << endl;
	out_file << "The error list is: " << endl;
	if (error_list.size() == 0)
	{
		out_file << "Congratulations! No lexical error is found." << endl;
	}
	else
	{
		for (unsigned int i = 0; i < error_list.size(); i++)
		{
			out_file << "At the line of " << error_list.at(i).first + 1 << ", the type of error is: " << error_list.at(i).second << endl;
		}
	}

	out_file << "The number of NUM is: " << num_num << endl;
	out_file << "The number of reserved character appeared in the source code is: " << reserved_num << endl;
	out_file << "The number of defined character is: " << symbol_table.size() << endl;
	out_file << "The number of error is: " << error_list.size() << endl;
}