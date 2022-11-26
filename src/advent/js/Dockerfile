FROM busybox:1.35

# Create a non-root user to own the files and run our server
RUN adduser -D static
USER static
WORKDIR /home/static

# Copy the static website
COPY advent.html .
COPY advent.js .
COPY advent.wasm .
COPY httpd.conf /etc

# Run BusyBox httpd
CMD ["busybox", "httpd", "-f", "-v", "-p", "80"]