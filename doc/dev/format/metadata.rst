.. _metadata_layout:

Metadata Layout
===============

The metadata in TagVFS wrapper files is stored usimg XML.

Items
-----

* Tags

  * List of tags
  * Recursion-capable

* File meta

  * Original filename of the wrapped file
  * Usual ``stat()`` info from wrapped file

    * What to do if user entered raw from stdin?

* User notes

  * Free text
  * HTML markup?

* Metadata extracted from file

  * Must include some indicator of metadata standard (was it taken from exif in
    a jpeg?  something else?)
  * How to handle updates to the file's metadata?

    * Either update the wrapper meta to match the contents, or leave the
      wrapper as original

  * Difficult to standardize what goes in this block... how to know if it's
    more XML, chunk of raw binary, or other?  Mime type?


.. code-block:: xml

   <tvm>
       <tags>
           <tag>Christmas</tag>
           <tag>Animals<tag>Dog</tag></tag>
       </tags>
       <stat>
           <name>Untitled_final (1) USE THIS ONE (1) (18).docx.jpg</name>
           <permission>0644</permission>
           <uid>1000</uid>
           <gid>1000</gid>
           <access>1234567890</access>
           <modify>1234567890</modify>
           <change>1234567890</change>
           <birth>1234567890</birth>
       </stat>
       <fromfile format="application/xml" from="exif">
           <ExposureProgram>28/5</ExposureProgram>
           <CameraModel>Banana</CameraModel>
       </fromfile>
       <user>
           Hello, world!
           <b>Bold, maybe?</b>
       </user>
   </tvm>

