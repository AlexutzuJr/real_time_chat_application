# Project Overview

In this project, our goal is to predict tomorrow's temperature using historical data. The process begins by downloading a dataset of local weather. Next, the data will be cleaned and prepared for machine learning. A system will be built to make historical predictions, followed by the addition of more predictors to improve the model. We will end with how to make next-day predictions.

**Project Steps**
* Download weather data
* Clean and graph data
* Create a testing framework
* Improve model accuracy

# Local Setup

## Installation

To follow this project, please install the following locally:

* JupyerLab
* Python 3.8+
* Python packages
    * pandas
    * scikit-learn

## Dataset

We will download our dataset from NOAA, a US government agency.  You can follow these instructions to download the data:

* Go to [NOAA Search](https://www.ncdc.noaa.gov/cdo-web/search)
* Enter the years you want the data for, and search for the closest airport to you.
* Click add to cart on the airport you want.
    * If there is no airport near you, try your city or country name instead.
* Go to the [cart](https://www.ncdc.noaa.gov/cdo-web/cart)
* Select the csv format and click continue.
* Select all of the checkboxes for data types.
* Enter your email and click continue.
* You will get an email with a link to download the data.
* Make sure to take a look at the [data documentation](https://www1.ncdc.noaa.gov/pub/data/cdo/documentation/GHCND_documentation.pdf) as well.
