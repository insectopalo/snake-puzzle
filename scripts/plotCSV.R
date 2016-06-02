library(rgl)

file1 <- "~/work/snake-puzzle/test/pascu.0001"


s1 <- read.csv(file1, header=FALSE)
plot3d(s1, type = "s", box=FALSE)
plot3d(s1, type = "l", add=TRUE, col="red")
