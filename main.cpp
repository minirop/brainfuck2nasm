#include <fstream>
#include <array>
#include <stack>
#include <vector>
#include <cstdint>
#include <cstdio>

/* loop emptying a cell */
#define CLEAR_LOOP '0'

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		return 0;
	}
	
	std::vector<char> program;
	
	std::ifstream file(argv[1]);
	while(!file.eof())
	{
		program.push_back(file.get());
	}
	file.close();
	
	std::ofstream output("output.asm");
	output << "section .data\n"
			<< "buffer: times 30000 db 0\n"
			<< "\n"
			<< "section .text\n"
			<< "	global _main\n"
			<< "	extern _putchar\n"
			<< "\n"
			<< "_main:\n"
			<< "	mov ecx, 0\n";
	
	std::vector<std::pair<char, int>> instructions;
	for(int i = 0;i < program.size();i++)
	{
		char c = program[i];
		switch(c)
		{
			// smashable instructions
			case '+':
			case '-':
			case '<':
			case '>':
				if(instructions.size() > 0 && instructions.back().first == c)
					instructions.back().second++;
				else
					instructions.push_back(std::make_pair(c, 1));
				break;
			// non-smashable instructions
			case '[':
				if(program[i+1] == '-' && program[i+2] == ']')
				{
					i += 2;
					instructions.push_back(std::make_pair(CLEAR_LOOP, 1));
					break;
				}
			case ']':
			case '.':
			case ',':
				instructions.push_back(std::make_pair(c, 1));
				break;
			default:
				/* comment */;
		}
	}
	
	int loopCount = 0;
	std::stack<int> currentLoops;
	for(auto const & pair : instructions)
	{
		switch(pair.first)
		{
			case '+':
				if(pair.second == 1)
					output << "	inc byte [buffer+ecx]\n";
				else
					output << "	add byte [buffer+ecx], " << pair.second << "\n";
				break;
			case '-':
				if(pair.second == 1)
					output << "	dec byte [buffer+ecx]\n";
				else
					output << "	sub byte [buffer+ecx], " << pair.second << "\n";
				break;
			case '<':
				if(pair.second == 1)
					output << "	dec ecx\n";
				else
					output << "	sub ecx, " << pair.second << "\n";
				break;
			case '>':
				if(pair.second == 1)
					output << "	inc ecx\n";
				else
					output << "	add ecx, " << pair.second << "\n";
				break;
			case '[':
				output << "	cmp byte [buffer+ecx], 0\n";
				output << "	je .loopend" << loopCount << "\n";
				output << " .loopstart" << loopCount << ":\n";
				currentLoops.push(loopCount);
				loopCount++;
				break;
			case ']':
				output << "	cmp byte [buffer+ecx], 0\n";
				output << "	jne .loopstart" << currentLoops.top() << "\n";
				output << " .loopend" << currentLoops.top() << ":\n";
				currentLoops.pop();
				break;
			case '.':
				output << "	push ecx\n";
				output << "	push dword [buffer+ecx]\n";
				output << "	call _putchar\n";
				output << "	add esp, 4\n";
				output << "	pop ecx\n";
				break;
			case ',':
				output << "	push ecx\n";
				output << "	call _putchar\n";
				output << "	mov byte [buffer+ecx], eax\n";
				output << "	pop ecx\n";
				break;
			case CLEAR_LOOP:
				output << "	mov byte [buffer+ecx], 0\n";
				break;
			default:
				/* comment */;
		}
	}
	output << "	ret\n";
	output.close();
	
	return 0;
}
