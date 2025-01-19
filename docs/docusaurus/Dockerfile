FROM node:20.4.0-alpine

WORKDIR /r-type/docs/

COPY package.json .

RUN yarn install

COPY . .

RUN yarn build

EXPOSE 3000


CMD ["yarn", "serve"]
