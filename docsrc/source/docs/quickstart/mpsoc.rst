######################################################
Quick Start Guide for Zynq |trade| UltraScale+ |trade| 
######################################################

The AMD **DPUCZDX8G** for Zynq |trade| Ultrascale+ |trade| is a configurable computation engine dedicated to convolutional neural networks. It supports a highly optimized instruction set, enabling the deployment of most convolutional neural networks. The following instructions will help you to install the software and packages required to support KV260/ZCU102/ZCU104.


*************
Prerequisites
*************

Host Requirements
=================

-  Confirm that your development machine meets the minimum :doc:`Host System Requirements <../reference/system_requirements>`.
-  Confirm that you have at least **100GB** of free space in the target partition.

Applicable Targets
==================

-  This quickstart is applicable to the following hardware platforms:

.. list-table::
   :widths: 60 40
   :header-rows: 1

   * - Product
     - Supported Target(s)

   * - Zynq |trade| Ultrascale+ |trade| Adaptable SoC Evaluation Boards
     - `ZCU102 <https://www.xilinx.com/zcu102>`__ / `ZCU104 <https://www.xilinx.com/zcu104>`__ / `Kria KV260 <https://www.xilinx.com/kria>`__



WSL 
===

This is an optional step intended to enable Windows users to evaluate Vitis |trade| AI. Although this is not a fully tested and supported flow, in most cases users will be able to execute this basic tutorial on Windows. 

The Windows Subsystem for Linux (WSL) can be installed from the command line. Open a Powershell prompt as an Administrator and execute the following command:

.. code-block:: Bash
		
	[Powershell] > wsl --install -d Ubuntu-20.04
	
	  

The user can list all distribution options available and select an alternate provided that it meets :doc:`Host System Requirements <../reference/system_requirements>`.

.. code-block:: Bash
		
	[Powershell] > wsl --list --online
	
	
You can start a specific distribution as follows:
	
.. code-block:: Bash
		
	[Powershell] > wsl -d Ubuntu-20.04


**********
Quickstart
**********


Clone the Vitis AI Repository
=============================

.. code-block:: Bash
		
	[Host] $ git clone https://github.com/Xilinx/Vitis-AI

	  
Install Docker
==============

.. note:: WSL users are advised to install Docker via `Docker desktop <https://docs.docker.com/desktop/wsl/>`__. WSL users can optionally leverage Docker using the command line flow below, however it has been found that the docker daemon doesn't start automatically in WSL. The instructions provided below may not work verbatim for WSL users. 

- Make sure that the Docker engine is installed according to the official Docker `documentation <https://docs.docker.com/engine/install/>`__.

- The Docker daemon always runs as the root user. Non-root users must be `added <https://docs.docker.com/engine/install/linux-postinstall/>`__ to the docker group. Do this now.

Verify Docker Installation
==========================

- Perform a quick and simple test of your Docker installation by executing the following command.  This command will download a test image from Docker Hub and run it in a container. When the container runs successfully, it prints a "Hello World" message and exits. 

.. code-block:: Bash
	
	[Host] $ docker run hello-world

- Finally, verify that the version of Docker that you have installed meets the minimum :doc:`Host System Requirements <../reference/system_requirements>` by running the following command

.. code-block:: Bash
	
	[Host] $ docker --version

Pull Vitis AI Docker
====================

In order to simplify this quickstart tutorial, we will utilize the Vitis-AI PyTorch CPU Docker to assess pre-built Vitis-AI examples, and subsequently perform quantization and compilation of our own model. The CPU docker image is generic, does not require the user to build the container, and has no specific GPU enablement requirements.  More advanced users can optionally skip this step and jump to the :doc:`Full Install Instructions <../install/install>` but we would recommend that new users start with this simpler first step.

Pull and start the latest Vitis AI Docker using the following commands:

.. code-block:: Bash
		
	[Host] $ docker pull xilinx/vitis-ai-pytorch-cpu:latest


Setup the Host
==============

It will be useful to you later on to have the cross-compiler installed.  This will allow you to compile target application code on your host machine inside Docker.  Run the following commands to install the cross-compilation environment.

.. note:: Perform these steps on your local host Linux operating system (not inside the docker container). By default, the cross compiler will be installed in ``~/petalinux_sdk_2022.2``.  The ~/petalinux_sdk_2022.2 path is recommended for the installation. Regardless of the path you choose for the installation, make sure the path has read-write permissions. In this quickstart, it is installed in ~/petalinux_sdk_2022.2

.. code-block:: Bash

	[Host] $ cd Vitis-AI/board_setup/mpsoc
	[Host] $ sudo chmod u+r+x host_cross_compiler_setup.sh
	[Host] $ ./host_cross_compiler_setup.sh
	  

When the installation is complete, follow the prompts and execute the following command:

.. code-block:: Bash

    [Host] $ source ~/petalinux_sdk_2022.2/environment-setup-cortexa72-cortexa53-xilinx-linux

.. note:: If you close the current terminal, you must re-execute the above instructions in the new terminal interface. Also, if you forget to run this command outside of Docker, you can execute it later from within.
	 

We are now ready to run the Docker container and cross compile one of the target applications.  Notice that the ``/workspace`` directory in Docker corresponds to your ``/Vitis-AI`` 
directory on the host.  Thus you will note that ``[Docker] /workspace/examples/vai_runtime/resnet50_pt = [Host] Vitis-AI/examples/vaiexamples/vai_runtime/resnet50_pt``.

.. code-block:: Bash
 
	[Host]   $ cd ../..
	[Host]   $ ./docker_run.sh xilinx/vitis-ai-pytorch-cpu:latest 

Activate the vitis-ai-pytorch conda enviornment. 

.. code-block:: Bash

	[Docker] $ conda activate vitis-ai-pytorch
	
Cross compile the ``resnet50_pt`` example.

.. code-block:: Bash

	[Docker] $ cd examples/vai_runtime/resnet50_pt
	[Docker] $ bash –x build.sh

If the compilation process does not report an error and the executable file ``resnet50_pt`` is generated, then the host environment is installed correctly. If an error is reported, double-check that you executed the ``source ~/petalinux....`` command.

Setup the Target
================

The Vitis AI Runtime packages, VART samples, Vitis-AI-Library samples, and models are built into the board image, enhancing the user experience. Therefore, the user need not install Vitis AI Runtime packages and model packages on the board separately.


1.  Make the target / host connections as shown in the images below.  Plug in the power adapter, ethernet cable, and a DisplayPort monitor (optional) and connect the USB-UART interface to the host.  If one is available, connect a USB webcam to the target.

.. note:: We recommend the Logitech BRIO for use with Vitis AI pre-built images.  The Logitech BRIO is capable of streaming RAW video at higher resolutions than most low-cost webcams.  When leveraging other low-cost webcams with the Vitis AI pre-built image, encoded video streams are actually decoded on the target's ARM APU which can reduce inference performance.


.. note:: The Kria KV260 can be leveraged with either HDMI or DisplayPort monitors.
	
.. image:: ../reference/images/kria_setup.png
	:width: 1300
		
.. note:: For ZCU102 and ZCU104, please use a display port monitor and reference the respective user guide to configure the target jumper and switch settings to factory defaults.  For both the ZCU102 and ZCU104, set Mode SW6 [4:1] = [OFF, OFF, OFF, ON] to boot from SD card.

.. image:: ../reference/images/ZCU102_setup.png
	:width: 1300

.. image:: ../reference/images/ZCU104_setup.png
	:width: 1300


2. Download the Vitis AI pre-built SD card image from the appropriate link:

   - `ZCU102 <https://www.xilinx.com/member/forms/download/design-license-xef.html?filename=xilinx-zcu102-dpu-v2022.2-v3.0.0.img.gz>`__
   - `ZCU104 <https://www.xilinx.com/member/forms/download/design-license-xef.html?filename=xilinx-zcu104-dpu-v2022.2-v3.0.0.img.gz>`__
   - `KV260 <https://www.xilinx.com/member/forms/download/design-license-xef.html?filename=xilinx-kv260-dpu-v2022.2-v3.0.0.img.gz>`__



3.  Use BalenaEtcher to burn the image file onto the SD card.

.. image:: ../reference/images/Etcher.png
    :width: 1300
    
4.  Insert the imaged SD card into the target and power up the board.
   
5.  Multiple consecutive UART ports will be enumerated (ie COM13,14,15,16).  Connect the serial terminal application of choice to the lowest of the consecutive ports (ie 13), using the parameters listed below.
   
	- Baud Rate: 115200
	- Data Bit: 8
	- Stop Bit: 1
	- No Parity

6.  The IP address for the target can be found with the command below.
 
.. code-block:: Bash
   
   [Target] $ ifconfig


.. image:: ../reference/images/ifconfig.png
    :width: 1300
	
If you are using a point-to-point connection or DHCP is not available, you can manually set the IP address:
	
.. code-block:: Bash

	[Target] $ ifconfig eth0 [TARGET_IP_ADDRESS]
	  	
		
7.  Next, connect to the board via SSH.  The password is 'root'

.. code-block:: Bash

    [Host] $ ssh -X root@[TARGET_IP_ADDRESS]
   
   
8. 	If you have not connected a DisplayPort monitor, it is recommended that you export the display.  If you do not do so, the examples will not run as expected.

.. code-block:: Bash

    [Target] $ export DISPLAY=[HOST_IP_ADDRESS]:0.0
	
   
9.  Download the model.
   
You can now select a model from the Vitis AI Model Zoo `Vitis AI Model Zoo <../workflow-model-zoo.html>`__.  Navigate to the  `model-list subdirectory  <https://github.com/Xilinx/Vitis-AI/tree/master/model_zoo/model-list>`__  and select the model that you wish to test. For each model, a YAML file provides key details of the model. In the YAML file there are separate hyperlinks to download the model for each supported target.  Choose the correct link for your target platform and download the model.

	a. Take the ResNet50 model as an example.

	.. code-block:: Bash
		
		[Host] $ cd /workspace
		[Host] $ wget https://www.xilinx.com/bin/public/openDownload?filename=resnet50-zcu102_zcu104_kv260-r3.0.0.tar.gz -O resnet50-zcu102_zcu104_kv260-r3.0.0.tar.gz
		
		
	b. Copy the downloaded file to the target using scp with the following command:
		
	.. code-block:: Bash

		[Host] $ scp resnet50-zcu102_zcu104_kv260-r3.0.0.tar.gz root@[TARGET_IP_ADDRESS]:~/
		
		
	c. Install the model package:

	.. code-block:: Bash
	   
		[Target] $ tar -xzvf resnet50-zcu102_zcu104_kv260-r3.0.0.tar.gz
		[Target] $ cp resnet50 /usr/share/vitis_ai_library/models -r

	
.. _mpsoc-run-vitis-ai-examples:


Run the Vitis AI Examples
=========================

1. Download `vitis_ai_runtime_r3.0.0_image_video.tar.gz <https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_runtime_r3.0.0_image_video.tar.gz>`__ to your host, and copy the file to the the target using scp:

.. code-block:: Bash

	[Host] $ scp vitis_ai_runtime_r3.0.0_image_video.tar.gz root@[TARGET_IP_ADDRESS]:~/
	  

2. Extract the ``vitis_ai_runtime_r3.0.0_image_video.tar.gz`` package on the target.

.. code-block:: Bash
   
	[Target] $ cd ~
	[Target] $ tar -xzvf vitis_ai_runtime_r3.0.0_image_video.tar.gz -C Vitis-AI/examples/vai_runtime
	

3. Navigate to the example directory on the target. Take ``resnet50`` as an example.

.. code-block:: Bash
	
	[Target] $ cd ~/Vitis-AI/examples/vai_runtime/resnet50
	

4. Run the example.

.. code-block:: Bash
	
	[Target] $ ./resnet50 /usr/share/vitis_ai_library/models/resnet50/resnet50.xmodel
	
		
5. An image should appear on the display connected to the target and the console should reflect the following output: 

.. code-block:: Bash	
		
	Image : 001.jpg
	top[0] prob = 0.982662  name = brain coral
	top[1] prob = 0.008502  name = coral reef
	top[2] prob = 0.006621  name = jackfruit, jak, jack
	top[3] prob = 0.000543  name = puffer, pufferfish, blowfish, globefish
	top[4] prob = 0.000330  name = eel

These results reflect the classification of a single test image located in the	``~/Vitis-AI/examples/vai_library/images``	directory.
			

****************
PyTorch Tutorial
****************
This tutorial assumes that Vitis AI has been installed and that the board has been configured as explained in the installation instructions above. For additional information on the Vitis AI Quantizer, Optimizer, or Compiler, please refer to the Vitis AI User Guide.


Quantizing the Model
====================

Quantization reduces the precision of network weights and activations to optimize memory usage and computational efficiency while maintaining acceptable levels of accuracy. Inference is computationally expensive and requires high memory bandwidths to satisfy the
low-latency and high-throughput requirements of Edge applications. Quantization and channel pruning techniques are employed to address these issues while achieving high performance and high energy efficiency with little degradation in accuracy. The Vitis AI Quantizer takes a 
floating-point model as an input and performs pre-processing (folds batchnorms and removes nodes not required for inference), and finally quantizes the weights/biases and activations to the given bit width.


1. Navigate to the cloned Vitis-AI directory and create a new workspace for your project.  Here you will store the test dataset, models, and python scripts required for quantization.

.. code-block:: Bash

	[Host] $ cd ~/Vitis-AI
	[Host] $ mkdir -p resnet18/model
	

2. Download the `ImageNet 1000 (mini) <https://www.kaggle.com/datasets/ifigotin/imagenetmini-1000/download?datasetVersionNumber=1>`__ dataset from Kaggle. This dataset is subset of the ILSVRC 2012-2017 dataset and comprises 1000 object classes, and contains 1,281,167 training, 50,000 validation, and 100,000 test images.  You will need to create a Kaggle account to access this dataset.  Move the downloaded `archive.zip` file into the created `/Vitis-AI/resnet18` folder and unzip the dataset.

.. code-block:: Bash

	[Host] $ cd resnet18
	[Host] $ unzip archive.zip
	
- Your workspace directory should reflect the following: 

::

	├── archive.zip
	│
	├── model    
	│                                    
	└── imagenet-mini
		├── train                    # Training images folder. Will not be used in this tutorial. 
		│   └─── n01440764           # Class folders to group images. 
		└── val                      # Validation images that will be used for quantization and evaluation of the floating point model. 
		    └─── n01440764
    


3. Navigate to the Vitis-AI directory and execute the following command to start Docker.
	
.. code-block:: Bash
	
	[Host] $ cd ..
	[Host] ./docker_run.sh xilinx/vitis-ai-pytorch-cpu:latest

* Note that when you start Docker appropriate as shown above, your ``/workspace`` folder will correspond to ``/Vitis-AI`` and your initial path in Docker will be ``/workspace``.  If you inspect ``docker_run.sh`` you can see that the -v option is leveraged which links the Docker file system to your Host file system.  Verify that you see the created ``/resnet18`` subfolder in your workspace:

.. code-block:: Bash
	
	[Docker] $ ls

4. Download the pre-trained resnet18 model from PyTorch to the docker environment and store it in the  ``model``  folder . This is the floating point (FP32) model that will be quantized to INT8 precision for deployment on the target.

.. code-block:: Bash

	[Docker] $ cd resnet18/model
	[Docker] $ wget https://download.pytorch.org/models/resnet18-5c106cde.pth -O resnet18.pth

.. note:: The `Vitis AI Model Zoo <../workflow-model-zoo.html>`__ also provides optimized deep learning models to speed up the deployment of deep learning inference on adaptable AMD platforms. For this tutorial we have chosen to use an open-source PyTorch model to showcase that models from the community can also be deployed.


5. Copy the example Vitis AI ResNet18 quantization script to your workspace. This script contains the Quantizer API calls that will be executed in order to quantize the model.

.. code-block:: Bash	

	[Docker] $ cd ..
	[Docker] $ cp ../src/vai_quantizer/vai_q_pytorch/example/resnet18_quant.py ./

* Your ``workspace/resnet18`` directory should reflect the following: 

::

	├── archive.zip
	│
	├── model 
	│   └── resnet18.pth             # ResNet18 floating point model downloaded from PyTorch.
	│                                    
	├── imagenet-mini
	│   ├── train                    # Training images folder. Will not be used in this tutorial. 
	│   │   └─── n01440764           # Class folders to group images. 
	│   └── val                      # Validation images that will be used for quantization and evaluation of the floating point model. 
	│       └─── n01440764
	│
	└── resnet18_quant.py            # Quantization python script.
 
 
* Inspect ``resnet18_quant.py``.  Observe the parser arguments that can be passed to the script via command line switches ``subset_len`` ``quant_mode`` ``data_dir`` and ``model_dir``.  We will set the ``data_dir`` and ``model_dir`` arguments to align with our directory structure.  If you wish to avoid extraneous typing and are manually entering these commands, you can simply edit the script to suit your use case.

.. code-block:: Bash	

	[Docker] $ vim resnet18_quant.py

* Use the sequence ``<esc> :q!`` to exit vim without saving.	

6. Run the command below to evaluate the accuracy of the floating point model.

.. code-block:: Bash	

	[Docker] $ python resnet18_quant.py --quant_mode float --data_dir imagenet-mini --model_dir model

* You should observe that the accuracy reported is similar to  ``top-1 / top-5 accuracy: 69.9975 / 88.7586``
	
7. Next, let's run the Model Inspector to confirm that this model should be compatible with the target DPU architecture.

.. code-block:: Bash	

	[Docker] $ python resnet18_quant.py --quant_mode float --inspect --target DPUCZDX8G_ISA1_B4096 --model_dir model
	

8. Run the command below to start quantization. Generally, 100-1000 images are required for quantization and the number of iterations can be controlled through the the ``subset_len`` data loading argument. In this case, 200 images are forward propagated through the network, and these images are chosen randomly from the validation image set.  Note that the displayed loss and accuracy that are output from this process are not representative of final model accuracy.


.. code-block:: Bash	

	[Docker] $ python resnet18_quant.py --quant_mode calib --data_dir imagenet-mini --model_dir model --subset_len 200

* On most host machines this command should complete in less than 1 minute even with the CPU-only Docker.  If you leverage the CUDA or ROCm Dockers on a compatible machine, the Quantization process will be accelerated considerably.  Let's take a look at the output:

.. code-block:: Bash	

	[Docker] $ cd quantize_result
	[Docker] $ ls

* If the command ran successfully, the output directory ``quantize_result`` will be generated, containing two important files:

	-``ResNet.py``
		The quantized vai_q_pytorch format model.
	-``Quant_info.json``
		Quantization steps of tensors. Retain this file for evaluation of the quantized model./
		
		
9. To evaluate the accuracy of the quantized model, return to the ``/resnet18`` directory run the following commands.  Note that on CPU-only host machines this command will take some time to complete (~20 minutes).  If you are in a hurry, you can skip this step and move to the next.

.. code-block:: Bash	

	[Docker] $ cd ..
	[Docker] $ python resnet18_quant.py --model_dir model --data_dir imagenet-mini --quant_mode test

You should observe that the accuracy reported will be similar to ``top-1 / top-5 accuracy: 69.1308 / 88.7076``.  The net accuracy loss due to quantization is less than 1%.

10. To generate the quantized ``.xmodel`` file that will subsequently be compiled for the DPU, run the following command with ``batch_size`` and ``subset_len`` arguments set to `1`. For model export, both of these parameters should be set `1` as multiple iterations are not required.

.. code-block:: Bash	

	[Docker] $ python resnet18_quant.py --quant_mode test --subset_len 1 --batch_size=1 --model_dir model --data_dir imagenet-mini --deploy

The resultant model `resnet18_pt.xmodel` can now be found in the `resnet18/resnet18_pt` folder.

Compile the model
=================
The Vitis AI Compiler compiles the graph operators as a set of micro-coded instructions that are executed by the DPU.  In this step, we will compile the ResNet18 model that we quantized in the previous step.


1. The compiler takes the quantized ``INT8.xmodel`` and generates the deployable ``DPU.xmodel`` by running the command below.  Note that you must modify the command to specify the appropriate ``arch.json`` file for your target.  For MPSoC targets, these are located in the folder ``/opt/vitis_ai/compiler/arch/DPUCZDX8G`` inside the Docker container.

.. code-block:: Bash
	
	[Docker] $ cd /workspace/resnet18
	[Docker] $ vai_c_xir -x quantize_result/ResNet_int.xmodel -a /opt/vitis_ai/compiler/arch/DPUCZDX8G/<Target ex:KV260>/arch.json -o resnet18_pt -n resnet18_pt 

- If compilation is successful, the ``resnet18_pt.xmodel`` file should be generated according to the specified DPU architecture. 

2. Create a new file with your text editor of choice and name the file ``resnet18_pt.prototxt``. Copy and paste the following lines of code:

.. code-block:: Bash

	model {
	   name : "resnet18_pt"
	   kernel {
		 name: "resnet18_pt_0"
		 mean: 103.53
		 mean: 116.28
		 mean: 123.675
		 scale: 0.017429
		 scale: 0.017507
		 scale: 0.01712475
	   }
	   model_type : CLASSIFICATION
	   classification_param {
		  top_k : 5
		  test_accuracy : false
		  preprocess_type : VGG_PREPROCESS
	   }
	}

- The ``.prototxt`` file is a Vitis |trade| AI configuration file that facilitates the uniform configuration management of model parameters. Please refer to the Vitis AI User Guide to learn more.


- We can now deploy the quantized and compiled model on the target. 

Model Deployment
================

1. Download the ``resnet18_pt`` folder from host to target using scp with the following command:

.. code-block:: Bash

	[Docker] $ scp -r resnet18_pt root@[TARGET_IP_ADDRESS]:/usr/share/vitis_ai_library/models/
	
* The model will be located under the ``/usr/share/vitis_ai_library/models/`` folder along with the other Vitis-AI model examples. 

2. The `vitis_ai_library_r3.0.0_images.tar.gz <https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r3.0.0_images.tar.gz>`__ and `vitis_ai_library_r3.0.0_video.tar.gz <https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r3.0.0_video.tar.gz>`__ packages 
contain test images and videos that can be leveraged to evaluate our quantized model and other pre-built Vitis-AI Library examples on the target. 

	a. Download the packages.
	
	.. code-block:: Bash

		[Docker] $ cd /workspace
		[Docker] $ wget https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r3.0.0_images.tar.gz -O vitis_ai_library_r3.0.0_images.tar.gz
		[Docker] $ wget https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_library_r3.0.0_video.tar.gz -O vitis_ai_library_r3.0.0_video.tar.gz
	
	b. SCP the files to the target.

	.. code-block:: Bash

		[Docker] $ scp -r vitis_ai_library_r3.0.0_images.tar.gz root@[TARGET_IP_ADDRESS]:~/
		[Docker] $ scp -r vitis_ai_library_r3.0.0_video.tar.gz root@[TARGET_IP_ADDRESS]:~/

	c. Untar the files on the target.

	.. code-block:: Bash

		[Target] $ tar -xzvf vitis_ai_library_r3.0.0_images.tar.gz -C ~/Vitis-AI/examples/vai_library/
		[Target] $ tar -xzvf vitis_ai_library_r3.0.0_video.tar.gz -C ~/Vitis-AI/examples/vai_library/

3. Enter the directory of the sample and then compile it.

.. code-block:: Bash

	[Target] $ cd ~/Vitis-AI/vai_library/samples/classification
	[Target] $ ./build.sh

4. Execute the single-image test application.

.. code-block:: Bash

	[Target] $ ./test_jpeg_classification resnet18_pt ~/Vitis-AI/examples/vai_library/samples/classification/images/002.jpg

If you wish to do so, you can copy the ``result.jpg`` file back to your host and review the output.  OpenCV function calls have been used to overlay the predictions.

5. To run the video example, run the following command.  To keep this simple we will use one of the Vitis AI video samples, but you should scp your own video clip to the target (webm / raw formats).

.. code-block:: Bash

	[Target] $ ./test_video_classification resnet18_pt ~/Vitis-AI/examples/vai_library/apps/seg_and_pose_detect/pose_960_540.avi -t 8

6. Users can run real time inference using a USB web camera connected to the target with the command below:

.. code-block:: Bash

	[Target] $ ./test_video_classification resnet18_pt 0 -t 8

- ``0`` corresponds to the first USB camera device node. If you have multiple USB cameras, the value is 1,2,3, etc.  ``-t`` corresponds to the number of threads.

- If you are uncertain, you can execute the following command to determine the device node for the camera connected to your target.

.. code-block:: Bash

	[Target] $ lsusb


7. The output will be displayed on the connected monitor.  Notice that the classifications are displayed frame-by-frame as an OpenCV overlay.  The performance will be limited by the refresh rate of the display.  Please see UG1354 for additional details.

.. image:: ../reference/images/Wallace.png
    :width: 1300


- Congratulations! You have successfully quantized, compiled, and deployed a pre-trained model on the target. 


.. |trade|  unicode:: U+02122 .. TRADEMARK SIGN
   :ltrim:
.. |reg|    unicode:: U+000AE .. REGISTERED TRADEMARK SIGN
   :ltrim: