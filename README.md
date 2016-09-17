This repository is shared under Apache License. See the License.txt for details.

Theft detection is composed of two phases:

1. Detection and tracking of moving objects in video.
2. Passing the moving objects to the frame analysis model and classification of the motion.

Motion recognition model

The model should be able to recognize pattern in space (motion instance in a given frame), as well as in time (motion pattern between frames). Traditionally, convolutional layers have been used for image analysis in the space, while recurrent layers are used to give the network ability to memorize events over the time. We deploy the same strategy and build a hybrid network that contains several convolutional layers in the beginning, followed by an LSTM layer. 
