clear;clc;

% V is a property of the illumination and of the camera.
V = [0,0,-1;0,0.2,-1;0,-0.2,-1;0.2,0,-1];
 
% read source image from figure folder 
im1 = imread('./figures/im1.png');
im2 = imread('./figures/im2.png');
im3 = imread('./figures/im3.png');
im4 = imread('./figures/im4.png');

[M,N] = size(im1);
g  = zeros(M,N,3);
surface_normal = zeros(M,N,3);
p = zeros(M,N);
q = zeros(M,N);
for i = 1:M
    for j = 1:N
        light = double([im1(i,j);im2(i,j);im3(i,j);im4(i,j)]);
        I = diag(light);
        g_value = (I * V)\(I * light);
        surface_normal(i,j,:) = g_value./norm(g_value,2);
        p(i,j) = -surface_normal(i,j,1)/surface_normal(i,j,3);
        q(i,j) = -surface_normal(i,j,2)/surface_normal(i,j,3);
        g(i,j,:) = g_value;
    end
end

%  use integration to get surface shape and show it in figure
f = zeros(M, N);
for m=2:M
    f(m,1) = f(m-1,1) + p(m,1); 
end
for m=1:M
    for n=2:N
        f(m,n) = f(m,n-1) + q(m,n);
    end
end
x=1:M;
y=1:N;
surf(x,y,f);

