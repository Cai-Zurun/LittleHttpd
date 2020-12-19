#include "httpresponse.h"

HttpResponse::HttpResponse(HttpRequest *httpRequest) {
    httpRequest_ = httpRequest;
    client_ = httpRequest->client_;
}

void HttpResponse::Response_() {
    struct stat st;
    char buf[1024];

    if (httpRequest_->error_ != httpRequest_->Correct) {
        if (httpRequest_->error_ == httpRequest_->Unimplemented) {
            Unimplemented_();
        }
        httpRequest_->client_->Destroy_();
        return;
    }

    if (stat(httpRequest_->path_, &st) == -1){
        NotFound_();
    } else {
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(httpRequest_->path_, "/index.html");
        if ((st.st_mode & S_IXUSR) ||
            (st.st_mode & S_IXGRP) ||
            (st.st_mode & S_IXOTH)    )
            httpRequest_->cgi_ = true;

        if (!httpRequest_->cgi_)
            ServeFile_(httpRequest_->path_);
        else
            ExecuteCgi_();
    }
    httpRequest_->client_->Destroy_();
}

void HttpResponse::Unimplemented_() {
    char buf[1024];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\nServer: littleHttpd/0.1.0\r\nContent-Type: text/html\r\n\r\n<HTML><HEAD><TITLE>Method Not Implemented\r\n</TITLE></HEAD>\r\n<BODY><P>HTTP request method not supported.\r\n</BODY></HTML>\r\n");
    client_->Write_(buf, strlen(buf));
}

void HttpResponse::NotFound_() {
    cout << "进入not found" << endl;
    char buf[1024];
    strcat(buf, "HTTP/1.0 404 NOT FOUND\r\nServer: littleHttpd/0.1.0\r\nContent-Type: text/html\r\n\r\n<HTML><TITLE>Not Found</TITLE>\r\n<BODY><P>NOT FOUND</BODY></HTML>\r\n");
    client_->Write_(buf, strlen(buf));
}

void HttpResponse::ServeFile_(char *filename) {
    FILE *resource = NULL;
    int charNum;
    int numchars=1;
    char buf[1024];

//    buf[0] = 'A'; buf[1] = '\0';
//    while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
//        numchars = httpRequest_.GetLine_(buf, sizeof(buf));
    do {
        charNum = httpRequest_->GetLine_(buf, sizeof(buf));
    }while (charNum > 0);

    resource = fopen(filename, "r");
    Headers_();
    Cat_(resource);
    fclose(resource);
}

void HttpResponse::Headers_() {
    char buf[1024];
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    client_->Write_(buf, strlen(buf));
    strcpy(buf, SERVER_STRING);
    client_->Write_(buf, strlen(buf));
    sprintf(buf, "Content-Type: text/html\r\n");
    client_->Write_(buf, strlen(buf));
    strcpy(buf, "\r\n");
    client_->Write_(buf, strlen(buf));
}

void HttpResponse::Cat_(FILE *resource) {
    char buf[1024];

    fgets(buf, sizeof(buf), resource);
    while (!feof(resource))
    {
        client_->Write_(buf, strlen(buf));
        fgets(buf, sizeof(buf), resource);
    }
}

void HttpResponse::ExecuteCgi_() {
    char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    pid_t pid;
    int status;
    int i;
    char c;
    int numchars = 1;
    int content_length = -1;

    buf[0] = 'A'; buf[1] = '\0';
    if (strcasecmp(httpRequest_->method_, "GET") == 0)
        while ((numchars > 0) && strcmp("\n", buf))
            numchars = httpRequest_->GetLine_(buf, sizeof(buf));
    else{
        numchars = httpRequest_->GetLine_(buf, sizeof(buf));

        while ((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = httpRequest_->GetLine_(buf, sizeof(buf));
        }

        if (content_length == -1) {
            BadRequest_();
            return;
        }
    }

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    client_->Write_(buf, strlen(buf));

    if (pipe(cgi_output) < 0) {
        CannotExecute_();
        return;
    }
    if (pipe(cgi_input) < 0) {
        CannotExecute_();
        return;
    }

    if ( (pid = fork()) < 0 ) {
        CannotExecute_();
        return;
    }

    if (pid == 0) {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(cgi_output[1], 1);
        dup2(cgi_input[0], 0);

        close(cgi_output[0]);
        close(cgi_input[1]);

        sprintf(meth_env, "REQUEST_METHOD=%s", httpRequest_->method_);

        putenv(meth_env);

        if (strcasecmp(httpRequest_->method_, "GET") == 0) {
            sprintf(query_env, "QUERY_STRING=%s", httpRequest_->queryString_);
            putenv(query_env);
        }
        else {
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }

        execl(httpRequest_->path_, NULL);
        exit(0);

    } else {
        close(cgi_output[1]);
        close(cgi_input[0]);

        if (strcasecmp(httpRequest_->method_, "POST") == 0)
            for (i = 0; i < content_length; i++) {
                client_->Read_(&c, 1);
                write(cgi_input[1], &c, 1);
            }

        while (read(cgi_output[0], &c, 1) > 0) {
            cout << c ;
            client_->Write_(&c, 1);
        }

        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }
}

void HttpResponse::BadRequest_() {
    char buf[1024];
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n\"Content-type: text/html\r\n\r\n<P>Your browser sent a bad request,such as a POST without a Content-Length.\r\n ");
    client_->Write_(buf, sizeof(buf));
}

void HttpResponse::CannotExecute_() {
    char buf[1024];
    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\nContent-type: text/html\r\n\r\n<P>Error prohibited CGI execution.\r\n");
    client_->Write_(buf, sizeof(buf));
}