A Neural Networks assignment - creating a network to recognize the digit '0' in images from the MNIST handwritten digit dataset.

Details about the network can be seen in the report.pdf file. In general, it works like this: there is one hidden layer of perceptrons, followed by a final neuron. Each perceptron recognizes (outputs 1) images whose normalized flattened representation lies above a certain hyperplane, chosen heuristically to maximize the chance of separating images of zero from those of other digits. The final neuron sums the outputs of the perceptrons and recognizes the image if the sum lies above a certain treshold.

The file data/train_hyperplanes_dump.txt contains preprocessed information about hyperplanes (one hyperplane per each train image representing a zero, not per neuron). The file data/train_neurons_dump.txt contains preprocessed information about the neurons.

To execute:

cd src 

cmake .

make

Then, to run all computations from scratch (which will overwrite preprecessed information):

./perceptron

On a PC that may easily take up to 40 hours. The process is not accelerated by a GPU.

To rerun network construction, but without recomputing the hyperplanes:

./perceptron -p

That can take about 12 hours. To run evaluation only:

./perceptron -t
