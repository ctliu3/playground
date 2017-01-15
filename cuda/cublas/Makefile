all: mat_vec_mul

mat_vec_mul: mat_vec_mul.cu
	nvcc -O2 -arch=sm_20 -lcublas -o mat_vec_mul mat_vec_mul.cu

%: %.cu
	nvcc -O2 -arch=sm_20 -o $@ $<

clean:
	rm -f mat_vec_mul
