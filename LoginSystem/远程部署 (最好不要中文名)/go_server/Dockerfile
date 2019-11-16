FROM golang:latest

WORKDIR $GOPATH
ADD . $GOPATH
RUN go build .

EXPOSE 8080

ENTRYPOINT ["./go"]







