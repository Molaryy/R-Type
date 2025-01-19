# build
FROM ubuntu

RUN apt update
RUN apt install -y libudev-dev software-properties-common lsb-release wget g++ gcc make git libx11-dev libopengl-dev libopenal-dev libflac-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev libvorbis-dev libfreetype-dev

# install cmake
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
RUN apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
RUN apt update
RUN apt install cmake -y
