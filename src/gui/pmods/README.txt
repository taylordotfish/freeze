This directory contains presentation models, abstract representations of the
views (in ../views). Presentation models are responsible for linking the models
with the views -- the views and models don't communicate directly with each
other, but rather go through the presentation model instead. Each view contains
a pointer to the corresponding presentation model.
