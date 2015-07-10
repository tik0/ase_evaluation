filename = 'hist.log';
cutoff = 2000;
bins = 30;

X = load(filename);
Y = X(X < cutoff);

hist(Y, bins);
