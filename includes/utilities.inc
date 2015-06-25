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
 *   The query to send to Solr.
 *
 * @return array
 *   An array containing all of the PIDs matching $query.
 */
function islandora_scg_query_solr($query) {
  if (is_null($query)) {
    return FALSE;
  }
  $solr_url = variable_get('islandora_solr_url', 'http://localhost:8080/solr');
  $pid_query = $solr_url . '/select?version=1.2&rows=1000000&wt=json&json.nl=map&q=' . $query;
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

/**
 * Gets the PIDs of a book or newspaper issue's descendents.
 *
 * @param string $pid
 *   The PID of the book or newspaper object.
 * @param string $cmodel
 *   The content model of the book or newspaper object.
 *
 * @return array
 *   An array contianing all the descendent objects' PIDs.
 */
function islandora_scg_get_descendents($pid, $cmodel) {
  if (!module_exists('islandora_paged_content')) {
    drush_set_error('Content model module not available', dt('Sorry, the Islandora Paged Content module is not enabled.'));
    exit;
  }
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $parent_object = islandora_object_load($pid);

  if ($cmodel == 'islandora:bookCModel') {
    $pages = islandora_paged_content_get_pages($parent_object);
    $page_pids = array_keys($pages);
    return $page_pids;
  }

  if ($cmodel == 'islandora:newspaperCModel') {
    if (!module_exists('islandora_newspaper')) {
      drush_set_error('Content model module not available', dt('Sorry, the Islandora Newspaper Solution Pack is not enabled.'));
      exit;
    }
    module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    $all_descendent_pids = array();
    // Get all the newspaper's issues.
    $issues = islandora_newspaper_get_issues($parent_object);
    $issue_keys = array_keys($issues);
    // For each issue in the newspaper, add its pages' PIDs to
    // $all_descendent_pids.
    foreach ($issue_keys as $issue_pid) {
      $issue_object = islandora_object_load($issue_pid);
      $issue_pages = islandora_paged_content_get_pages($issue_object);
      $issue_page_keys = array_keys($issue_pages);
      $all_descendent_pids = array_merge($all_descendent_pids, $issue_page_keys);
    }
    return $all_descendent_pids;
  }
}
