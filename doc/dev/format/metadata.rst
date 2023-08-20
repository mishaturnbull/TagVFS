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


.. code-block:: xml

   <tvm>
       <tags>
           <tag>Christmas</tag>
           <tag>Animals<tag>Dog</tag></tag>
       </tags>
       <filedata>
           <permission>0644</permission>
           <uid>1000</uid>
           <gid>1000</gid>
           <access>1234567890</access>
           <modify>1234567890</modify>
           <change>1234567890</change>
           <birth>1234567890</birth>
       </filedata>
       <user>
           Hello, world!
           <b>Bold, maybe?</b>
       </user>
   </tvm>

