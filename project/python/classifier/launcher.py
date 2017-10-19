import glob as d
import scipy.cluster.vq as vq
import cv2
import time
import random as r
import numpy as np
import cPickle

def getImages(url):
    print("Start to read all images in directory {}".format(url))
    ini = time.time()
    exts = [".jpg", ".jpeg", ".png",".avi"]
    imgPaths = []
    for ext in exts:
        imgPaths.extend(d.glob(url+"*"+ext))
    end = time.time()
    print("\t-Total Images: {}".format(len(imgPaths)))
    print("Done in : {} seconds.\n".format(end-ini))
    return imgPaths


def getInformation(imgPaths):
    print("Start to recollect information of {} images...".format(len(imgPaths)))
    ini = time.time()
    detector=cv2.xfeatures2d.SIFT_create()
    keys, desc= [], []
    tDesc = 0
    for path in imgPaths:
        ima=cv2.imread(path)
        #gray=cv2.cvtColor(ima,cv2.COLOR_BGR2GRAY)
        kpts,des=detector.detectAndCompute(ima,None)
        keys.append(kpts)
        desc.append(des)
        tDesc += len(des)
    end = time.time()
    print("\t-Total descriptors: {}".format(tDesc))
    print("\t-Total keypoints: {}".format(tDesc))
    print("Done in : {} seconds.\n".format(end-ini))
    return keys, desc

"""
    Distortion is defined as the sum of the squared
    differences between the observations and the corresponding centroid.
"""
def getCentroidsKmeans(desc, k, fileName):
    r.seed((1000,2000))
    ini = time.time()
    print("Start to obtain {} centroids of the descriptors with k-means...".format(k))
    sizeDescriptor=desc[0].shape[1]
    print("\t-Size of each descriptor: {} ".format(sizeDescriptor))
    #Take a number of samples of the descriptors
    numSamples = len(desc) / 10
    samples=np.zeros((numSamples,sizeDescriptor),dtype=np.float32)
    for i in range(numSamples):
        #take a random descriptor
		samples[i,:]=r.choice(r.choice(desc))

    codebook,distortion=vq.kmeans(samples,k,1)
    print("\t-The distortion making in the clusters are {}".format(distortion))
    print("\t-Saving the centroids in disk with filename: {} ".format(fileName))
    cPickle.dump(codebook, open(fileName, "wb"))
    end = time.time()
    print("Done in : {} seconds.\n".format(end-ini))




if __name__ == "__main__":

    trainImages = getImages("data/train/")
    kpTrain, descTrain = getInformation(trainImages)
    getCentroidsKmeans(descTrain, 10, "centroids.df")
