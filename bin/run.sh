SOURCE_FOLDER=$(pwd)
DEST_FOLDER=~

docker run --rm -v "$SOURCE_FOLDER:$DEST_FOLDER" -it epitechcontent/epitest-docker /bin/bash -c "useradd $(whoami) && su - $(whoami)"