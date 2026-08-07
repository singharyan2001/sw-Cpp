# SETUP & INTALL DEPENDENCIES FOR PYTHON & GO

## Python

### To install gRPC and protobuf tools in system wide (but avoiding venv for this due to extra work in cmake)

```markdown
# Use pipx (Easiest for Global Tools):
pipx is a tool specifically designed to install Python command-line applications globally while keeping their dependencies safely isolated in the background.

1. Install pipx via the system package manager:
sudo apt install pipx
pipx ensurepath

2. Install the gRPC tools:
pipx install grpcio-tools
```

### Installing grpc and protobuf dev tools for python

```markdown
# 1. Create a Virtual Environment
python3 -m venv .pyvenv

# 2. Activate the environment
source .pyvenv/bin/activate

# Note: to exit -> deactivate

# 3. Install the tools safely inside the sandbox
pip install grpcio-tools
```

---

## GO

### Install the Go Language Support

```markdown
# Install Go Compiler
sudo apt update
sudo apt install golang-go

# Verify if installed
go version

# Set up your Go Environment Path
When you use go install to download tools, Go puts the resulting executable binaries in a specific folder: ~/go/bin.
If this folder isn't in your system's PATH, CMake and your terminal won't be able to find the plugins.

Run this command to add it to your current terminal session:
export PATH="$PATH:$(go env GOPATH)/bin"

Note: To make this permanent, you should add that exact export line to the bottom of your ~/.bashrc or ~/.zshrc file.

# Install the Go Protobuf and gRPC Plugins
# Installs the plugin that generates the basic data structures (messages)
go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

# Installs the plugin that generates the gRPC Service and Client Stub interfaces
go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest

# Verify the Installation
which protoc-gen-go
which protoc-gen-go-grpc

```
