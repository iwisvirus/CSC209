#include "request.h"
#include "response.h"
#include <string.h>


/******************************************************************************
 * ClientState-processing functions
 *****************************************************************************/
ClientState *init_clients(int n) {
    ClientState *clients = malloc(sizeof(ClientState) * n);
    for (int i = 0; i < n; i++) {
        clients[i].sock = -1;  // -1 here indicates available entry
    }
    return clients;
}

/* 
 * Remove the client from the client array, free any memory allocated for
 * fields of the ClientState struct, and close the socket.
 */
void remove_client(ClientState *cs) {
    if (cs->reqData != NULL) {
        free(cs->reqData->method);
        free(cs->reqData->path);
        for (int i = 0; i < MAX_QUERY_PARAMS && cs->reqData->params[i].name != NULL; i++) {
            free(cs->reqData->params[i].name);
            free(cs->reqData->params[i].value);
        }
        free(cs->reqData);
        cs->reqData = NULL;
    }
    close(cs->sock);
    cs->sock = -1;
    cs->num_bytes = 0;
}


/*
 * Search the first inbuf characters of buf for a network newline ("\r\n").
 * Return the index *immediately after* the location of the '\n'
 * if the network newline is found, or -1 otherwise.
 * Definitely do not use strchr or any other string function in here. (Why not?)
 */
int find_network_newline(const char *buf, int inbuf) {
    if (buf == NULL){
        return -1;
    }
    for (int i = 0; i < inbuf - 1; i++){
        if (buf[i] == '\r' && buf[i + 1] == '\n'){
            return i + 2;
        }
    }
    return -1;
}

/*
 * Removes one line (terminated by \r\n) from the client's buffer.
 * Update client->num_bytes accordingly.
 *
 * For example, if `client->buf` contains the string "hello\r\ngoodbye\r\nblah",
 * after calling remove_line on it, buf should contain "goodbye\r\nblah"
 * Remember that the client buffer is *not* null-terminated automatically.
 */
void remove_buffered_line(ClientState *client) {
    // IMPLEMENT THIS

    // if client buf is empty
    if (client -> num_bytes == 0){
        memset(&client -> buf, '\0', MAXLINE);
    }

    // index of new line
    int new_line_index = find_network_newline(client -> buf, client -> num_bytes);

    // update num_bytes
    client -> num_bytes = client -> num_bytes - new_line_index;

    // move it to the beginning of buf
    memmove(client -> buf, client -> buf + new_line_index, client -> num_bytes);
    memset(client -> buf + client -> num_bytes, '\0', MAXLINE - client -> num_bytes);

}


/*
 * Read some data into the client buffer. Append new data to data already
 * in the buffer.  Update client->num_bytes accordingly.
 * Return the number of bytes read in, or -1 if the read failed.

 * Be very careful with memory here: there might be existing data in the buffer
 * that you don't want to overwrite, and you also don't want to go past
 * the end of the buffer, and you should ensure the string is null-terminated.
 * 
 * Each call to read from client, should try to read in the 
 * available space in the buffer 
 * (no more than MAXLINE - client->num_bytes). 
 *  
 */
int read_from_client(ClientState *client) {
    if (client -> num_bytes == MAXLINE - 1){
        perror("Buffer full.\n");
        return -1;
    }
    int bytes = read(client -> sock, client -> buf + client -> num_bytes, MAXLINE - 1 - client -> num_bytes);
    if (bytes < 0){
        perror("Cannot read from socket.\n");
        return -1;
    }
    client -> num_bytes = client -> num_bytes + bytes;
    client -> buf[client -> num_bytes] = '\0';

    return bytes;
}


/*****************************************************************************
 * Parsing the start line of an HTTP request.
 ****************************************************************************/
// Helper function declarations.
void parse_query(ReqData *req, const char *str);
void update_fdata(Fdata *f, const char *str);
void fdata_free(Fdata *f);
void log_request(const ReqData *req);


/* If there is a full line (terminated by a network newline (CRLF)) 
 * then use this line to initialize client->reqData
 * Return 0 if a full line has not been read, 1 otherwise.
 */
int parse_req_start_line(ClientState *client) {
    //IMPLEMENT THIS

    // if there is a full line terminated by a network newline (CRLF)
    int full_line = find_network_newline(client -> buf, client -> num_bytes);
    //double check 
    if (full_line == -1){
        perror("Full line not terminated by a network newline.\n");
        return 0;
    }

    char *first_line = malloc(MAXLINE);
    if (first_line == NULL){
        fprintf(stderr, "Memory not allocated for first line.\n");
        exit(1);
    }

    strcpy(first_line, client -> buf);
    first_line[full_line - 2] = '\0';

    // client -> reqdata -> method = "GET" or "POST"
    char method[5];
    memset(method, '\0', sizeof(method));
    client -> reqData = malloc(sizeof(ReqData));
    if (client -> reqData == NULL){
        fprintf(stderr, "Memory not allocated for reqData.\n");
        free(client -> reqData);
        exit(1);
    }
    memset(client -> reqData, '\0', sizeof(ReqData));
    // GET
    if (strstr(first_line, GET) != NULL){
        strcpy(method, GET);
        client -> reqData -> method = malloc(sizeof(char) * strlen(method) + 1);
        strcpy(client -> reqData -> method, method);

    // POST
    }else if(strstr(first_line, POST) != NULL){
        strcpy(method, POST);
        client -> reqData -> method = malloc(sizeof(char) * strlen(method) + 1);
        if (client -> reqData -> method == NULL){
        fprintf(stderr, "Memory not allocated for method.\n");
        exit(1);
    }
        strcpy(client -> reqData -> method, method);

    // not both
    }else{
        perror("Method cannot be read. Should be POST or GET.\n");
        return 0;
    }

    // client -> reqdata -> path 
    if (strstr(first_line, "/") != NULL){
        // if there is / and ?
        if (strstr(first_line, "?") != NULL){
            if (strcmp(client -> reqData -> method, GET) == 0){
                // setting reqData -> path
                char target[MAXLINE];
                char *path_token_with_slash = strstr(first_line, "/");
                char *path_token_with_question = strtok(path_token_with_slash, "?");

                memset(target, '\0', sizeof(target));
                strcpy(target, path_token_with_question);

                client -> reqData -> path = malloc(sizeof(char) * strlen(target) + 1);
                if (client -> reqData -> path == NULL){
                    fprintf(stderr, "Memory not allocated in filter.\n");
                    exit(1);
                }
                strcpy(client -> reqData -> path, target);

                // setting reqData -> param
                char name_value[MAXLINE];
                char *param_token = strtok(NULL, "?");

                memset(name_value, '\0', sizeof(name_value));
                strncpy(name_value, param_token, strlen(param_token) - strlen(" HTTP/1.1"));
                // name_value[strlen(name_value)] = '\0';
                parse_query(client -> reqData, name_value);


            }
            else{
                perror("Method should be GET to have name-value pairs./n");
                return 0;
            }
        
        // if there is / but no ?
        }else {
            char target[MAXLINE];
            char *token = strstr(first_line, "/"); 

            memset(target, '\0', MAXLINE);
            strncpy(target, token, strlen(token) - strlen(" HTTP/1.1"));
            // target[strlen(target)] = '\0';
            client -> reqData -> path = malloc(sizeof(char) * strlen(target));
            if (client -> reqData -> path == NULL){
                fprintf(stderr, "Memory not allocated in copy_filter.\n");
                exit(1);
    }
            strcpy(client -> reqData -> path, target);
        }
    // if there is no /
    }else {
        perror("HTTP request start line not correct.\n");
        return 0;
    }

    free(first_line);

    // This part is just for debugging purposes.
    ReqData *req = client -> reqData;
    log_request(req);
    return 1;
}


/*
 * Initializes req->params from the key-value pairs contained in the given 
 * string.
 * Assumes that the string is the part after the '?' in the HTTP request target,
 * e.g., name1=value1&name2=value2.
 */
void parse_query(ReqData *req, const char *str) {

    // IMPLEMENT THIS
    char copy[strlen(str) + 1];
    strcpy(copy, str);

    char *token = strtok(copy, "&");
    int query = 0;

    while(token != NULL && query < MAX_QUERY_PARAMS){
        // name1=value1
        char name_value[strlen(token) + 1];
        strcpy(name_value, token);

        char name[MAXLINE];
        char value[MAXLINE];
        char *v = strstr(name_value, "="); 

        memset(value, '\0', sizeof(value));
        strcpy(value, v + 1);

        memset(name, '\0', sizeof(name));
        strncpy(name, name_value, strlen(name_value) - strlen(v));

        if(name != NULL){
            req -> params[query].name = malloc(sizeof(char) * strlen(name) + 1);
            if (req -> params[query].name == NULL){
                fprintf(stderr, "Memory not allocated in copy_filter.\n");
                exit(1);
                }
            req -> params[query].value = malloc(sizeof(char) * strlen(value) + 1);
            if (req -> params[query].value == NULL){
                fprintf(stderr, "Memory not allocated in copy_filter.\n");
                exit(1);
            } 
            strcpy(req -> params[query].name, name);
            strcpy(req -> params[query].value, value);
            query ++;
        token = strtok(NULL, "&");
    }
}
}



/*
 * Print information stored in the given request data to stderr.
 */
void log_request(const ReqData *req) {
    fprintf(stderr, "Request parsed: [%s] [%s]\n", req->method, req->path);
    for (int i = 0; i < MAX_QUERY_PARAMS && req->params[i].name != NULL; i++) {
        fprintf(stderr, "  %s -> %s\n", 
                req->params[i].name, req->params[i].value);
    }
}


/******************************************************************************
 * Parsing multipart form data (image-upload)
 *****************************************************************************/

char *get_boundary(ClientState *client) {
    int len_header = strlen(POST_BOUNDARY_HEADER);

    while (1) {
        int where = find_network_newline(client->buf, client->num_bytes);
        if (where > 0) {
            if (where < len_header || strncmp(POST_BOUNDARY_HEADER, client->buf, len_header) != 0) {
                remove_buffered_line(client);
            } else {
                // We've found the boundary string!
                // We are going to add "--" to the beginning to make it easier
                // to match the boundary line later
                char *boundary = malloc(where - len_header + 1);
                strncpy(boundary, "--", where - len_header + 1);
                strncat(boundary, client->buf + len_header, where - len_header - 1);
                boundary[where - len_header] = '\0';
                return boundary;
            }
        } else {
            // Need to read more bytes
            if (read_from_client(client) <= 0) {
                // Couldn't read; this is a bad request, so give up.
                return NULL;
            }
        }
    }
    return NULL;
}


char *get_bitmap_filename(ClientState *client, const char *boundary) {
    int len_boundary = strlen(boundary);

    // Read until finding the boundary string.
    while (1) {
        int where = find_network_newline(client->buf, client->num_bytes);
        if (where > 0) {
            if (where < len_boundary + 2 ||
                    strncmp(boundary, client->buf, len_boundary) != 0) {
                remove_buffered_line(client);
            } else {
                // We've found the line with the boundary!
                remove_buffered_line(client);
                break;
            }
        } else {
            // Need to read more bytes
            if (read_from_client(client) <= 0) {
                // Couldn't read; this is a bad request, so give up.
                return NULL;
            }
        }
    }

    int where = find_network_newline(client->buf, client->num_bytes);

    client->buf[where-1] = '\0';  // Used for strrchr to work on just the single line.
    char *raw_filename = strrchr(client->buf, '=') + 2;
    int len_filename = client->buf + where - 3 - raw_filename;
    char *filename = malloc(len_filename + 1);
    strncpy(filename, raw_filename, len_filename);
    filename[len_filename] = '\0';

    // Restore client->buf
    client->buf[where - 1] = '\n';
    remove_buffered_line(client);
    return filename;
}

/*
 * Read the file data from the socket and write it to the file descriptor
 * file_fd.
 * You know when you have reached the end of the file in one of two ways:
 *    - search for the boundary string in each chunk of data read 
 * (Remember the "\r\n" that comes before the boundary string, and the 
 * "--\r\n" that comes after.)
 */
int save_file_upload(ClientState *client, const char *boundary, int file_fd) {
    // Read in the next two lines: Content-Type line, and empty line
    remove_buffered_line(client);
    remove_buffered_line(client);
    // IMPLEMENT THIS

    char *last_line = malloc(sizeof(char) * (strlen(boundary) + 6));
    strcpy(last_line, "\r\n");
    strcat(last_line, boundary);
    strcat(last_line, "--\r\n");

    int w = write(file_fd, client -> buf, sizeof(char) * client -> num_bytes);
    if (w < 0){
        perror("Could not write first data to file descriptor.\n");
        exit(1);
    }

    while(1){
        int bytes_read = read(client -> sock, client -> buf, MAXLINE);
        if (bytes_read < 0){
            perror("Could not read from socket.\n");
            exit(1);
        }
        char *compare = strstr(client -> buf, last_line);
        if (compare != NULL){
            int w = write(file_fd, client -> buf, sizeof(char) * bytes_read);
            if (w < 0){
                perror("Could not write last line to file descriptor.\n");
                exit(1);
            }
            break;
        } else{
            int w = write(file_fd, client -> buf, sizeof(char) * bytes_read);
            if (w < 0){
                perror("Could not write data to file descriptor.\n");
                exit(1);
            }
        }
    }

    free(last_line);
    return 0;
}
