% This program finds a decision boundary to classify samples using multiple
% features using minimum squared error procedure


% take input from training file
% need to arrange features columnwise in training file
% need to keep first feature to 1 always
% so second to (last of column - 1) is the features
% last column is the true class
fileId = fopen('training_set.txt','r');
feature_vectors = fscanf(fileId,'%d %d %d', [4 Inf]);
[samples,features] = size(feature_vectors);
fclose(fileId);

% to adapt the equation multiply with -1
only_features = feature_vectors(:,:);
for i = 1 : samples
    for j = features : features
      only_features(i,j) = only_features(i,j)*-1;
    end
end

% genate equation with syms
Q = sym(sym('w%d%d',size(only_features)));
Y = sum(sym(only_features).*Q,features);

[x,y] = size(Y);
X = 0;
% square the equation for each sample
for i = 1 : x
   X = X + sum(Y(i,:)).^2;
end

% geneate real equations for each samples
A = [sym(w0) sym(w0) sym(w0) sym(w0) sym(w0)];
syms w11 w21 w31 w41 w51
X = subs(X,[w11,w21,w31,w41,w51],A);

A = [sym(w1) sym(w1) sym(w1) sym(w1) sym(w1)];
syms w12 w22 w32 w42 w52
X = subs(X,[w12,w22,w32,w42,w52],A);

A = [sym(w2) sym(w2) sym(w2) sym(w2) sym(w2)];
syms w13 w23 w33 w43 w53
X = subs(X,[w13,w23,w33,w43,w53],A);

A = [sym(w3) sym(w3) sym(w3) sym(w3) sym(w3)];
syms w14 w24 w34 w44 w54
X = subs(X,[w14,w24,w34,w44,w54],A);

A = [sym(w4) sym(w4) sym(w4) sym(w4) sym(w4)];
syms w15 w25 w35 w45 w55
X = subs(X,[w15,w25,w35,w45,w55],A);

% substitute the last numeric value
% here for two features
syms w3
X = subs(X,w3,1);

% do partial differentiation
diffw1 = diff(X,w0);
diffw2 = diff(X,w1);
diffw3 = diff(X,w2);
diffw4 = diff(X,w3);
diffw5 = diff(X,w4);

% make equation to solve and solve
% for two features
eqn1 = diffw1 == 0;
eqn2 = diffw2 == 0;
eqn3 = diffw3 == 0;
sol = solve([eqn1, eqn2, eqn3], [w0, w1, w2]);

% get output decision boundary
%for two features
E = [sol.w0 sol.w1 sol.w2];
syms x1 x2
Eq = sym(w0) + sym(w1)*sym(x1) + sym(w2)*sym(x2);
subs(Eq,[w0,w1,w2],E)









