import glob as d
import scipy.cluster.vq as vq
import cv2
import time
import random as r
import numpy as np
import cPickle

def getImages(url, classes):
    print("Start to read all images in directory {}".format(url))
    ini = time.time()
    exts = [".jpg", ".jpeg", ".png",".avi"]
    imgClasses = {}

    for i in range(0, len(classes)):
        imgClasses[classes[i]] = []

    for c in classes:
        totalImages = 0;
        pa = []
        for ext in exts:
            paths = d.glob("{}/{}/*{}".format(url, c, ext))
            if (len(paths)>0):
                totalImages += len(paths)
                pa.extend(paths)
        print("\t-Total Images class {}: {}".format(c, totalImages))
        imgClasses[c]= pa

    end = time.time()
    print("Done in : {} seconds.\n".format(end-ini))
    return imgClasses


def getInformation(classes):
    print("Start to recollect information of the images...")
    ini = time.time()
    detector=cv2.xfeatures2d.SIFT_create()
    tDesc = 0
    descClass = {}

    for key in classes:
        paths=  classes[key]
        keys, desc= [], []
        for path in paths:
            ima=cv2.imread(path)
            #gray=cv2.cvtColor(ima,cv2.COLOR_BGR2GRAY)
            kpts,des=detector.detectAndCompute(ima,None)
            keys.append(kpts)
            desc.append(des)
            tDesc += len(des)
        descClass[key] = desc

    end = time.time()
    print("\t-Total descriptors: {}".format(tDesc))
    print("\t-Total keypoints: {}".format(tDesc))
    print("Done in : {} seconds.\n".format(end-ini))
    return descClass

"""
    Distortion is defined as the sum of the squared
    differences between the observations and the corresponding centroid.
"""
def getCentroidsKmeans(classes, k, fileName):
    r.seed((1000,2000))
    ini = time.time()
    print("Start to obtain {} centroids of the descriptors with k-means...".format(k))
    centroidsClass = {}
    for c in classes:
        desc  = classes[c]
        sizeDescriptor=desc[0].shape[1]
        print("\t\t-Size of each descriptor: {} for class: {}".format(sizeDescriptor, c))
        #Take a number of samples of the descriptors
        numSamples = len(desc)
        print("\t\t-Total number of samples {}".format(numSamples))
        samples=np.zeros((numSamples,sizeDescriptor),dtype=np.float32)
        for i in range(numSamples):
            #take a random descriptor
    		samples[i,:]=r.choice(r.choice(desc))
        codebook,distortion=vq.kmeans(samples,k,1)
        centroidsClass[c] = [codebook, distortion]
        print("\t\t-The distortion making in the clusters are {}\n".format(distortion))

    print("\t-Saving the centroids in disk with filename: {} ".format(fileName))
    cPickle.dump(centroidsClass, open(fileName, "wb"))
    end = time.time()

    print("Done in : {} seconds.\n".format(end-ini))
    return centroidsClass

def knnClassifier(train, test):
    ini = time.time()
    print("Start to train and test knn classifier...")
    knn = cv2.ml.KNearest_create()
    trainData, responses = [], []
    idClasses = {}
    invClasses = {}
    for i in range(0, len(classes)):
        idClasses[classes[i]] = i
        invClasses[i] = classes[i]

    for c in train:
        centroids , distortion = train[c][0], train[c][1]
        for centroid in centroids:
            trainData.append(centroid)
        responses.extend([[idClasses[c]]]*len(centroids))
    nptrain = np.array(trainData, dtype=np.float32)
    npres = np.array(responses, dtype=np.float32)
    knn.train(nptrain, cv2.ml.ROW_SAMPLE, npres)

    numNeighbors = 10
    total = 0.0
    good = 0.0
    for c in test:
        descs = test[c]
        for  d in descs:
            total += 1
            newcomer = np.array([d[0]], dtype=np.float32)
            ret, results, neighbours ,dist = knn.findNearest(newcomer, numNeighbors)
            classto =  invClasses[int(results[0][0])]
            isTrue = classto == c
            if (isTrue): good += 1
    print ("\taccuracy Knn Classifer :  {}% ".format((good/total)*100))
    end = time.time()
    print("Done in : {} seconds.\n".format(end-ini))



def normalBayesClassifier(train, test):
    ini = time.time()
    print("Start to train and test bayes classifier...")
    bayes = cv2.ml.NormalBayesClassifier_create()
    trainData, responses = [], []
    idClasses = {}
    invClasses = {}
    for i in range(0, len(classes)):
        idClasses[classes[i]] = i
        invClasses[i] = classes[i]

    for c in train:
        centroids , distortion = train[c][0], train[c][1]
        for centroid in centroids:
            trainData.append(centroid)
        responses.extend([[idClasses[c]]]*len(centroids))
    nptrain = np.array(trainData)
    npres = np.array(responses)
    bayes.train(nptrain, cv2.ml.ROW_SAMPLE, npres)

    total = 0.0
    good = 0.0
    for c in test:
        descs = test[c]
        for  d in descs:
            total += 1
            newcomer = np.array([d[0]], dtype=np.float32)
            _, res , probs = bayes.predictProb(newcomer)
            classto =  invClasses[int(res[0][0])]
            isTrue = classto == c
            if (isTrue): good += 1
    print ("\taccuracy bayes Classifer :  {}% ".format((good/total)*100))
    end = time.time()
    print("Done in : {} seconds.\n".format(end-ini))


if __name__ == "__main__":
    classes = ["knive", "carrot"]

    trainImages = getImages("data/train", classes)
    descTrain = getInformation(trainImages)
    centTrain = getCentroidsKmeans(descTrain, 100, "centroids.df")

    testImages = getImages("data/test", classes)
    descTest = getInformation(testImages)

    knnClassifier(centTrain, descTest)
    normalBayesClassifier(centTrain, descTest)
