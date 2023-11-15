FLAGS = -Wall -std=gnu99 -g

all: scale edge_detection gaussian_blur greyscale copy image_filter

copy: copy.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

greyscale: greyscale.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

gaussian_blur: gaussian_blur.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm

edge_detection: edge_detection.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm	

scale: scale.o bitmap.o
	gcc ${FLAGS} -o $@ $^ -lm		

image_filter: image_filter.o
	gcc ${FLAGS} -o $@ $^ -lm

%.o: %.c bitmap.h
	gcc ${FLAGS} -c $<

clean:
	rm *.o image_filter copy greyscale gaussian_blur edge_detection scale

test:
	mkdir images -p
	./copy < dog.bmp > images/dog_copy.bmp
	./greyscale <dog.bmp > images/greyscale.bmp
	./gaussian_blur <dog.bmp > images/gaussian_blur.bmp
	./edge_detection <dog.bmp > images/edge_detection.bmp
	./scale 3 <dog.bmp > images/scale.bmp
	./gaussian_blur < dog.bmp | ./gaussian_blur | ./gaussian_blur | ./greyscale | ./scale 2 > images/dog_piped.bmp
	./image_filter dog.bmp images/dog_piped2.bmp ./gaussian_blur ./gaussian_blur ./gaussian_blur ./greyscale "./scale 2"
