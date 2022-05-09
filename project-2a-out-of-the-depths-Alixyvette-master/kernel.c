//Converting the following lines to C
//movb $'O',  0xb8000 //this address of the frame buffer
//movb $0x0f, 0xb8001 //
//movb $'K',  0xb8002
//movb $0x0f, 0xb8003

void myCKernel(){
	char* ptr = 0xb8000;
	*ptr = 'O';
	ptr++;
	*ptr = 0x0f;
	ptr++;
	*ptr = 'K';
	ptr++;
	*ptr = 0x0f;
	while(1);
}
