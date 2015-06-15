<?php

/**
 * @file
 * Utility functions used by the Islandora Sample Content Generator.
 */

/**
 * Queries Islandora's Solr index to get a list of PIDs.
 *
 * The object is the one corresponding to the alias and pointer.
 *
 * @param string $query
 *   The CONTENTdm collection alias
 */
function islandora_scg_query_solr($query) {
  if (is_null($query)) {
    return FALSE;
  }
  $solr_url = variable_get('islandora_solr_url', 'http://localhost:8080/solr');
  $pid_query = $solr_url . '/select?version=1.2&wt=json&json.nl=map&q=' . $query;
  $raw_result = drupal_http_request($pid_query);
  if ($raw_result->code != 200) {
    return FALSE;
  }
  $query_result = json_decode($raw_result->data);
  if ($query_result->response->numFound < 1) {
    return FALSE;
  }

  // If we haven't returned yet, filter out the PIDs to return as a list.
  $pid_list = array();
  foreach ($query_result->response->docs as $doc) {
    $pid_list[] = $doc->PID;
  }
  return $pid_list;
}
