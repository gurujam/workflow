#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

#ifdef LIBXML_TREE_ENABLED
//#include "STCMAssembly.h"

//static void parse_component (xmlNode * a_node, Component c) {
static void parse_component (xmlNode * a_node) {
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {

      if (strcmp(cur_node->name, "dataIn") == 0){
        xmlAttr* attribute = cur_node->properties;
        //récupère la valeur des attributs nom et type
        xmlChar* name;
        name = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
        attribute = attribute->next;
        xmlChar* type;
        type = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
        attribute = attribute->next;
        if(attribute){
          xmlChar* set;
          set = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
          //c.addInPort(new InPort(name, type, set));
          printf("dataIn name= %s, type= %s, set=%s\n", name, type, set);
          xmlFree(set);
        }
        else{
          //c.addInPort(new InPort(name, type));
          printf("dataIn name= %s, type= %s\n", name, type);
        }

        xmlFree(name);
        xmlFree(type);
      }

      else if (strcmp(cur_node->name, "dataOut") == 0){
        xmlAttr* attribute = cur_node->properties;
        xmlChar* name;
        name = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
        attribute = attribute->next;
        xmlChar* type;
        type = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        //c.addOutPort(new OutPort(name, type));
        printf("dataOut name = %s, type = %s\n", name, type);

        xmlFree(name);
        xmlFree(type);
      }

      else{
        //preuve que l'on ne boucle pas pour rien
        printf("\n");
      }
    }
  }
}

//static void parse_inout (xmlNode * a_node, InOut inout) {
static void parse_inout (xmlNode * a_node) {
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {

      if (strcmp(cur_node->name, "setPort") == 0){
        xmlAttr* attribute = cur_node->properties;
        xmlChar* in;
        in = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
        attribute = attribute->next;
        xmlChar* out;
        out = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        //SetPort lien = SetPort(in, out);
        //inout.addInOut(lien);
        printf("setPort in = %s, out = %s\n", in, out);
      }
      //else if(strcmp(cur_node->name, "unsetPort") == 0){}
      else{
        //preuve que l'on ne boucle pas pour rien
        printf("\n");
      }
    }
  }
}

//static void parse_configPort (xmlNode * a_node, ConfigPort ports) {
static void parse_configPort (xmlNode * a_node) {
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {

      if (strcmp(cur_node->name, "inout") == 0){
        //InOut inout = InOut();
        //parse_inout(cur_node->children, &inout);
        //ports.setInOut(inout);
        printf("parse_inout()\n");
        parse_inout(cur_node->children);
      }
      //else if(strcmp(cur_node->name, "clientserver") == 0){}
      else{
        //preuve que l'on ne boucle pas pour rien
        printf("\n");
      }
    }
  }
}

//static void parse_declaration (xmlNode * a_node, STCMAssembly assembly) {
static void parse_declaration (xmlNode * a_node) {
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node && cur_node->type == XML_ELEMENT_NODE) {
      if (strcmp(cur_node->name, "component") == 0){
        printf("parse_component()\n");
        //on récupère son nom
        xmlAttr* attribute = cur_node->properties;
        xmlChar* name;
        name = xmlNodeListGetString(cur_node->doc, attribute->children, 1);
        //on crée l'objet
        //Component c = Component(name);
        printf("Component name = %s\n", name);
        //on parse l'objet component (ses ports dataIn et dataOut)
        //parse_component(cur_node->children, &c);
        //assembly.addComponent(&c);
        parse_component(cur_node->children);

        xmlFree(name);
      }
      else if (strcmp(cur_node->name, "instance") == 0){
        printf("parse_instance()\n");
        //on récupère ses attributs
        xmlAttr* attribute = cur_node->properties;
        xmlChar* name;
        name = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        attribute = attribute->next;
        xmlChar* componentRef;
        componentRef = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        attribute = attribute->next;
        xmlChar* path;
        path = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        attribute = attribute->next;
        xmlChar* signature;
        signature = xmlNodeListGetString(cur_node->doc, attribute->children, 1);

        //assembly.addInstrance(Instance(name, componentRef, path, signature));
        printf("Instance name = %s, componentRef = %s, path = %s, signature = %s\n", name, componentRef, path, signature);

        xmlFree(name); xmlFree(componentRef);
        xmlFree(path); xmlFree(signature);
      }
      else if (strcmp(cur_node->name, "configPort") == 0){
        printf("parse_configPort()\n");
        //TODO: crée l'objet configPort, son inout et ses setPorts
        //ConfigPort confports = ConfigPort();
        //parse_configPort(cur_node->children, confports);
        //assembly.setPorts(confports);
        parse_configPort(cur_node->children);
      }
      else{
        //preuve que l'on ne boucle pas pour rien
        printf("\n");
      }
    }
  }
}

//static void parse_instruction (xmlNode * a_node, STCMAssembly stcmassembly) {
static void parse_instruction (xmlNode * a_node) {
  printf("....\n");
}

//static void parse_stcm (xmlNode * a_node, STCMAssembly stcmassembly) {
static void parse_stcm (xmlNode * a_node) {
  xmlNode *cur_node = NULL;

  for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {

      if (strcmp(cur_node->name, "declare") == 0){
        printf("parse_declaration()\n");
        //crée l'objet component avec ses dataIn et dataOut
        //parse_declaration(cur_node->children, &stcmassembly);
        parse_declaration(cur_node->children);
      }
      if (strcmp(cur_node->name, "instruction") == 0){
        printf("parse_instruction()\n");
        //TODO: crée l'objet instruction avec ses attributs
        //parse_instruction(cur_node->children, &stcmassembly);
        parse_instruction(cur_node->children);
      }
    }
    //parse_stcm(cur_node->children, &stcmassembly);
    parse_stcm(cur_node->children);
  }
}


int main(int argc, char **argv)
{
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;

  if (argc != 2)
    return(1);

  LIBXML_TEST_VERSION

  doc = xmlReadFile(argv[1], NULL, 0);

  if (doc == NULL) {
    printf("error: could not parse file %s\n", argv[1]);
    return(1);
  }

  root_element = xmlDocGetRootElement(doc);

  //STCMAssembly assembly;
  //parse_stcm(root_element, &assembly);
  parse_stcm(root_element);

  //assembly.wiggle();

  xmlFreeDoc(doc);
  xmlCleanupParser();
  return(0);
}
#else
int main(void) {
  fprintf(stderr, "Tree support not compiled in\n");
  exit(1);
}
#endif
