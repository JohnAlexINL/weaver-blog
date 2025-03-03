char run_buffer[4*1024];
int run(const char *command) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) { return -1; }
    size_t bytes_read = fread(run_buffer, 1, sizeof(run_buffer) - 1, fp);
    run_buffer[bytes_read] = '\0';
    pclose(fp); return bytes_read;
}