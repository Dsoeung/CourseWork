
//Insert a new record from pointer into the position of an existing record, appending existing record

int replaceRec(int fd, int iRecNum, struct custData * record) {
	int i;
	off_t seekPos;
	size_t recsize = sizeof(struct custData);
	struct custData buffer;
	seekPos = recsize * iRecNum;
// Seek to the position of the indicated
// record (i.e., the record at offset iRecNum).
if(lseek(fd, seekPos, SEEK_SET) == -1) {
	perror("seek failed");
	return EXIT_FAILURE;
}
// Read the record into our local buffer in memory.
if(read(fd, buffer, recsize) != recsize) {
	perror("read failed");
	return EXIT_FAILURE;
}
// Seek to the end of the file (this is the position
// immediately after the last byte. i.e., if we write
// anything at this position, it will be appended to
// the end of the file.
if(lseek(fd, 0, SEEK_END) == -1) {
	perror("seek failed");
	return EXIT_FAILURE;
}
// Write the record from our buffer to the file
// (i.e., append the record to the end of the file)
if(write(fd, buffer, recsize) != recsize) {
	perror("write failed");
	return EXIT_FAILURE;
}
// Seek again to the position of indicated record
// (the record whose offset is iRecNum).
if(lseek(fd, seekPos, SEEK_SET) == -1) {
	perror("seek failed");
	return EXIT_FAILURE;
}
// Finally, overwrite the old record in the file with
// the record that was passed by pointer into this
// function.
if(write(fd, record, recsize) != recsize) {
	perror("write failed");
	return EXIT_FAILURE;
}
return 0;